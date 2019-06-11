/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ELEMENTAL_DT_DIST_MATRIX_HPP
#define ELEMENTAL_DT_DIST_MATRIX_HPP

#include <elemental/config.hpp>
#include <elemental/fwd/dt/dist_matrix.hpp>
#include <elemental/dt/matrix.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_distribution.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/make.hpp>
#include <El.hpp>
#include <type_traits>
#include <initializer_list>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;

template<
	typename Ring = double,
	El::Dist Columnwise = El::MC,
	El::Dist Rowwise = El::MR,
	El::DistWrap Wrapping = El::ELEMENT
>
struct dist_matrix {
	template<typename Ring_, El::Dist Columnwise_, El::Dist Rowwise_, El::DistWrap Wrapping_>
	friend struct dist_matrix;
	
	template<
		typename Ring_ = Ring,
		El::Dist Columnwise_ = Columnwise,
		El::Dist Rowwise_ = Rowwise,
		El::DistWrap Wrapping_ = Wrapping,
		typename std::enable_if_t<
			std::is_same_v<std::remove_const_t<Ring>, Ring_>
		>* = nullptr
	>
	dist_matrix(El::DistMatrix<Ring_, Columnwise_, Rowwise_, Wrapping_> data) : data_{data} {
		BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	}
	
	dist_matrix(El::Grid const& grid, El::Int m, El::Int n) : data_{grid} {
		data_.Resize(m, n);
		El::Zero(data_);
	}
	
	template<
		typename Ring_ = Ring,
		El::Dist Columnwise_ = Columnwise,
		El::Dist Rowwise_ = Rowwise,
		El::DistWrap Wrapping_ = Wrapping,
		typename std::enable_if_t<
			std::is_same_v<std::remove_const_t<Ring>, Ring_>
		>* = nullptr
	>
	dist_matrix(dist_matrix<Ring_, Columnwise_, Rowwise_, Wrapping_> const& o) : data_{o.data_} {}
	
	dist_matrix(dist_matrix const&) = default;
	dist_matrix(dist_matrix &&) = default;
	
	dist_matrix&
	operator=(dist_matrix const&) = default;
	dist_matrix&
	operator=(dist_matrix &&) = default;
	
	auto
	m() const {
		return data_.Height();
	}
	
	auto
	n() const {
		return data_.Width();
	}

	mpl::matrix_size<El::Int, El::Int>
	size() const {
		return { m(), n() };
	}
	
	decltype(auto)
	data() & { return (data_); };

	decltype(auto)
	data() const& { return (data_); }; 

	decltype(auto)
	data() && { return HBRS_MPL_FWD(data_); }; 

private:
	El::DistMatrix<
		std::remove_const_t<Ring> /* elemental handles constness using a boolean El::Matrix<>.Locked() */,
		Columnwise, Rowwise, Wrapping
	> data_;
};

ELEMENTAL_NAMESPACE_END

BOOST_HANA_NAMESPACE_BEGIN

/* Ref.: Elemental/include/El/core.hpp */
template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
struct tag_of<elemental::dist_matrix<Ring, Columnwise, Rowwise, Wrapping>> {
	using type = elemental::dist_matrix_tag;
};

template <>
struct make_impl<elemental::dist_matrix_tag> {
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	static elemental::dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	apply(
		El::Grid const& grid,
		basic_type<Ring>,
		hbrs::mpl::matrix_distribution<
			integral_constant<El::Dist, Columnwise>,
			integral_constant<El::Dist, Rowwise>,
			integral_constant<El::DistWrap, Wrapping>
		>,
		hbrs::mpl::matrix_size<El::Int, El::Int> sz
	) {
		return { grid, sz.m(), sz.n() };
	}
	
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	static elemental::dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	apply(El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> dmat) {
		return { dmat };
	}
	
	template <typename Ring>
	static elemental::dist_matrix<Ring, El::STAR, El::STAR, El::ELEMENT>
	apply(
		El::Grid const& grid,
		El::Matrix<Ring> local
	) {
		El::DistMatrix<Ring, El::STAR, El::STAR, El::ELEMENT> dmat{grid};
		dmat.Resize(local.Height(), local.Width());
		dmat.Matrix() = local;
		return { dmat };
	}
	
	template <typename Ring>
	static decltype(auto)
	apply(
		El::Grid const& grid,
		elemental::matrix<Ring> local
	) {
		return apply(grid, std::move(local).data());
	}
};

BOOST_HANA_NAMESPACE_END

#endif // !ELEMENTAL_DT_DIST_MATRIX_HPP
