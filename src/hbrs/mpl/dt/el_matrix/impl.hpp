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

#ifndef HBRS_MPL_DT_EL_MATRIX_IMPL_HPP
#define HBRS_MPL_DT_EL_MATRIX_IMPL_HPP

#include "fwd.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/config.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/make.hpp>

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/translate_index.hpp>
#include <hbrs/mpl/detail/copy_matrix.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
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

HBRS_MPL_NAMESPACE_BEGIN

template<typename Ring>
struct el_matrix {
	template<
		typename Ring_ = Ring,
		typename std::enable_if_t<
			std::is_same_v<std::remove_const_t<Ring>, Ring_>
		>* = nullptr
	>
	el_matrix(El::Matrix<Ring_> data) : data_{data} {
		//NOTE: This assertion does not hold always, e.g. El::Matrix<double>{El::Matrix<double> const}.Locked() == true!
		BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	}
	
	el_matrix(El::Int m, El::Int n) : data_{m,n} {
		El::Zero(data_);
	}
	
	el_matrix(el_matrix const&) = default;
	el_matrix(el_matrix &&) = default;
		
	el_matrix&
	operator=(el_matrix const&) = default;
	el_matrix&
	operator=(el_matrix &&) = default;
	
	auto
	m() const {
		return data_.Height();
	}
	
	auto
	n() const {
		return data_.Width();
	}

	matrix_size<El::Int, El::Int>
	size() const {
		return { m(), n() };
	}
	
	decltype(auto)
	at(matrix_index<El::Int, El::Int> const& i) {
		return at_(data_, i);
	}
	
	decltype(auto)
	at(matrix_index<El::Int, El::Int> const& i) const {
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
	static at_(Matrix && m, matrix_index<El::Int, El::Int> const& i) {
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

HBRS_MPL_NAMESPACE_END

BOOST_HANA_NAMESPACE_BEGIN

template <typename Ring> 
struct tag_of< hbrs::mpl::el_matrix<Ring> > { 
	using type = hbrs::mpl::el_matrix_tag;
}; 

template <>
struct make_impl<hbrs::mpl::el_matrix_tag> {
	template <typename Ring>
	static hbrs::mpl::el_matrix<Ring> 
	apply(basic_type<Ring>, hbrs::mpl::matrix_size<El::Int, El::Int> sz) {
		return {sz.m(), sz.n()};
	}
	
	template <typename Ring> 
	static hbrs::mpl::el_matrix<Ring>
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
		hbrs::mpl::el_matrix<_Ring_> m{sz.m(), sz.n()};
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
		hbrs::mpl::el_matrix<_Ring_> m{sz.m(), sz.n()};
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
		hbrs::mpl::sm<std::array<Ring, Length>, hbrs::mpl::matrix_size<M, N>, Order> const& b
	) {
		typedef std::remove_cv_t<Ring> _Ring_;
		hbrs::mpl::matrix_size<El::Int, El::Int> sz{b.size()};
		hbrs::mpl::el_matrix<_Ring_> m{sz.m(), sz.n()};
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
		hbrs::mpl::el_matrix<_Ring_> m{sz.m(), sz.n()};
		return hbrs::mpl::detail::copy_matrix(data, hbrs::mpl::storage_order_c<Order>, m);
	}
	
	template<
		typename Ring,
		std::size_t Rows,
		std::size_t Columns,
		hbrs::mpl::storage_order Order
	>
	static constexpr auto
	apply(hbrs::mpl::ctsam<Ring, Rows, Columns, Order> const& a) {
		using namespace hbrs::mpl;
		typedef std::decay_t<Ring> _Ring_;
		matrix_size<El::Int, El::Int> sz = make_matrix_size(Rows, Columns);
		el_matrix<_Ring_> b{sz.m(), sz.n()};
		return hbrs::mpl::detail::copy_matrix(a, b);
	}
	
	template<
		typename Ring,
		hbrs::mpl::storage_order Order
	>
	static constexpr auto
	apply(hbrs::mpl::rtsam<Ring, Order> const& a) {
		using namespace hbrs::mpl;
		typedef std::decay_t<Ring> _Ring_;
		matrix_size<El::Int, El::Int> sz = (*hbrs::mpl::size)(a);
		el_matrix<_Ring_> b{sz.m(), sz.n()};
		return hbrs::mpl::detail::copy_matrix(a, b);
	}
}; 

BOOST_HANA_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_DT_EL_MATRIX_IMPL_HPP
