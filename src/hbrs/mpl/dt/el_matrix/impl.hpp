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

#ifndef ELEMENTAL_DT_MATRIX_HPP
#define ELEMENTAL_DT_MATRIX_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/make.hpp>

#include <elemental/dt/exception.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/translate_index.hpp>
#include <hbrs/mpl/detail/copy_matrix.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <boost/hana/type.hpp>
#include <El.hpp>
#include <type_traits>
#include <initializer_list>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;

template<typename Ring>
struct matrix {
	template<
		typename Ring_ = Ring,
		typename std::enable_if_t<
			std::is_same_v<std::remove_const_t<Ring>, Ring_>
		>* = nullptr
	>
	matrix(El::Matrix<Ring_> data) : data_{data} {
		//NOTE: This assertion does not hold always, e.g. El::Matrix<double>{El::Matrix<double> const}.Locked() == true!
		BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	}
	
	matrix(El::Int m, El::Int n) : data_{m,n} {
		El::Zero(data_);
	}
	
	matrix(matrix const&) = default;
	matrix(matrix &&) = default;
		
	matrix&
	operator=(matrix const&) = default;
	matrix&
	operator=(matrix &&) = default;
	
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
	at(mpl::matrix_index<El::Int, El::Int> const& i) {
		return at_(data_, i);
	}
	
	decltype(auto)
	at(mpl::matrix_index<El::Int, El::Int> const& i) const {
		return at_(data_, i);
	}
	
	decltype(auto)
	data() & { return (data_); };

	decltype(auto)
	data() const& { return (data_); }; 

	decltype(auto)
	data() && { return HBRS_MPL_FWD(data_); }; 

private:
	template<typename Matrix>
	decltype(auto)
	static at_(Matrix && m, mpl::matrix_index<El::Int, El::Int> const& i) {
		BOOST_ASSERT(i.m() >= 0 && i.m() < HBRS_MPL_FWD(m).Height());
		BOOST_ASSERT(i.n() >= 0 && i.n() < HBRS_MPL_FWD(m).Width());
		
		if constexpr(std::is_const_v<Ring> || std::is_const_v<std::remove_reference_t<Matrix>>) {
			return *HBRS_MPL_FWD(m).LockedBuffer(i.m(), i.n());
		} else {
			return *HBRS_MPL_FWD(m).Buffer(i.m(), i.n());
		}
	}
	
	El::Matrix<std::remove_const_t<Ring>> data_; /* elemental handles constness using a boolean El::Matrix<>.Locked() */
};

ELEMENTAL_NAMESPACE_END

BOOST_HANA_NAMESPACE_BEGIN

template <typename Ring> 
struct tag_of< elemental::matrix<Ring> > { 
	using type = elemental::matrix_tag;
}; 

template <>
struct make_impl<elemental::matrix_tag> {
	template <typename Ring>
	static elemental::matrix<Ring> 
	apply(basic_type<Ring>, hbrs::mpl::matrix_size<El::Int, El::Int> sz) {
		return {sz.m(), sz.n()};
	}
	
	template <typename Ring> 
	static elemental::matrix<Ring>
	apply(El::Matrix<Ring> data) {
		return {data}; 
	}
	
	template <
		typename Ring,
		typename M,
		typename N,
		hbrs::mpl::storage_order Order,
		typename std::enable_if_t<
			std::is_convertible<M,El::Int>::value && std::is_convertible<N,El::Int>::value
		>* = nullptr
	>
	static constexpr auto
	apply(
		hbrs::mpl::sm<hbrs::mpl::rtsav<Ring>, hbrs::mpl::matrix_size<M, N>, Order> const& b
	) {
		typedef std::remove_cv_t<Ring> _Ring_;
		hbrs::mpl::matrix_size<El::Int, El::Int> sz{b.size()};
		elemental::matrix<_Ring_> m{sz.m(), sz.n()};
		return hbrs::mpl::detail::copy_matrix(b, m);
	}
	
	template <
		typename Ring,
		std::size_t Length,
		typename M,
		typename N,
		hbrs::mpl::storage_order Order,
		typename std::enable_if_t<
			std::is_convertible<M,El::Int>::value && std::is_convertible<N,El::Int>::value
		>* = nullptr
	>
	static constexpr auto
	apply(
		hbrs::mpl::sm<hbrs::mpl::ctsav<Ring, Length>, hbrs::mpl::matrix_size<M, N>, Order> const& b
	) {
		typedef std::remove_cv_t<Ring> _Ring_;
		hbrs::mpl::matrix_size<El::Int, El::Int> sz{b.size()};
		elemental::matrix<_Ring_> m{sz.m(), sz.n()};
		return hbrs::mpl::detail::copy_matrix(b, m);
	}
	
	template <
		typename Ring,
		hbrs::mpl::storage_order Order
	>
	static constexpr auto
	apply(
		std::initializer_list<Ring> data,
		hbrs::mpl::matrix_size<El::Int, El::Int> sz,
		hbrs::mpl::storage_order_<Order>
	) {
		typedef std::remove_cv_t<Ring> _Ring_;
		elemental::matrix<_Ring_> m{sz.m(), sz.n()};
		return hbrs::mpl::detail::copy_matrix(data, hbrs::mpl::storage_order_c<Order>, m);
	}
}; 

BOOST_HANA_NAMESPACE_END

#endif // !ELEMENTAL_DT_MATRIX_HPP