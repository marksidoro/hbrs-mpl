/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_SELECT_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_SELECT_IMPL_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
#include <hbrs/mpl/dt/el_vector/fwd.hpp>
#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
#include <hbrs/mpl/dt/el_matrix_range.hpp>

#include <hbrs/mpl/detail/add_const.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <boost/assert.hpp>

#include <El.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct select_impl_column_vector {
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Vector && v,
		mpl::range<El::Int, El::Int>  const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(v),
			El::IR{rng.first(), rng.last()+1}
		);
	}
	
	template<
		typename Vector,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value &&
			std::is_lvalue_reference<Vector>::value
		>* = nullptr
	>
	auto
	operator()(Vector && v, El::IR const& rng) const {
		return make_column_vector(v.data()(rng, El::ALL));
	}
	
	template<typename Ring>
	auto
	operator()(column_vector<Ring> && v, El::IR const& rng) const {
		using namespace hbrs::mpl;
		
		El::Matrix<Ring> b;
		El::GetSubmatrix(std::move(v).data(), rng, El::ALL, b);
		
		return make_column_vector(b);
	}
};

struct select_impl_matrix {
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, dist_matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		mpl::range<
			mpl::matrix_index<El::Int, El::Int>,
			mpl::matrix_index<El::Int, El::Int>
		>  const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(a),
			std::make_pair(
				El::IR{rng.first().m(), rng.last().m()+1},
				El::IR{rng.first().n(), rng.last().n()+1}
			)
		);
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, dist_matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		std::pair<
			mpl::matrix_index<El::Int, El::Int>,
			mpl::matrix_size<El::Int, El::Int>
		>  const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(a),
			std::make_pair(
				El::IR{
					hana::first(rng).m(), 
					hana::first(rng).m()+hana::second(rng).m()
				},
				El::IR{
					hana::first(rng).n(), 
					hana::first(rng).n()+hana::second(rng).n()
				}
			)
		);
	}
	
	template<typename Ring>
	matrix<Ring>
	operator()(matrix<Ring> & a, std::pair<El::IR, El::IR> const& rng) const {
		return {El::View(a.data(), hana::first(rng), hana::second(rng))};
	}
	
	template<typename Ring>
	matrix<Ring const>
	operator()(matrix<Ring> const& a, std::pair<El::IR, El::IR> const& rng) const {
		return {El::LockedView(a.data(), hana::first(rng), hana::second(rng))};
	}
	
	template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	operator()(dist_matrix<Ring, Columnwise, Rowwise, Wrapping> & a, std::pair<El::IR, El::IR> const& rng) const {
		return {El::View(a.data(), hana::first(rng), hana::second(rng))};
	}
	
	template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	dist_matrix<Ring const, Columnwise, Rowwise, Wrapping>
	operator()(dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a, std::pair<El::IR, El::IR> const& rng) const {
		return {El::LockedView(a.data(), hana::first(rng), hana::second(rng))};
	}
	
	static bool
	same_size(mpl::matrix_size<El::Int, El::Int> const& sz, std::pair<El::IR, El::IR> const& rng) {
		return hana::first(rng).beg == 0 && hana::first(rng).end == sz.m() &&
			hana::second(rng).beg == 0 && hana::second(rng).end == sz.n();
	}
	
	template<typename Ring>
	matrix<Ring>
	operator()(matrix<Ring> && a, std::pair<El::IR, El::IR> const& rng) const {
		if (same_size(mpl::size(a), rng)) {
			return HBRS_MPL_FWD(a);
		} else {
			typedef std::decay_t<Ring> _Ring_;
			El::Matrix<_Ring_> b;
			El::GetSubmatrix(HBRS_MPL_FWD(a).data(), hana::first(rng), hana::second(rng), b);
			return {b};
		}
	}
	
	template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	operator()(dist_matrix<Ring, Columnwise, Rowwise, Wrapping> && a, std::pair<El::IR, El::IR> const& rng) const {
		if (same_size(mpl::size(a), rng)) {
			return HBRS_MPL_FWD(a);
		} else {
			El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> b{a.data().Grid()};
			El::GetSubmatrix(HBRS_MPL_FWD(a).data(), hana::first(rng), hana::second(rng), b);
			return {b};
		}
	}
};

struct select_impl_dist_vector {
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<Vector>, dist_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<Vector>, dist_row_vector_tag >
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Vector && v,
		mpl::range<El::Int, El::Int> const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(v),
			El::IR{rng.first(), rng.last()+1}
		);
	}
	
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<Vector>, dist_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<Vector>, dist_row_vector_tag >
		>* = nullptr
	>
	auto
	operator()(Vector && v, El::IR const& rng) const {
		return hana::make<hana::tag_of_t<Vector>>(
			mpl::select(
				make_dist_matrix(v.data()),
				std::make_pair(rng, El::ALL)
			).data()
		);
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SELECT_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                        \
		elemental::detail::select_impl_column_vector{},                                                                \
		elemental::detail::select_impl_dist_vector{},                                                                  \
		elemental::detail::select_impl_matrix{}                                                                        \
	)

#endif // !HBRS_MPL_FN_SELECT_IMPL_ELEMENTAL_HPP
