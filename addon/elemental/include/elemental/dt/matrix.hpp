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

#pragma once

#ifndef ELEMENTAL_DT_MATRIX_HPP
#define ELEMENTAL_DT_MATRIX_HPP

#include <elemental/config.hpp>
#include <elemental/fwd/dt/matrix.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/make.hpp>

#include <elemental/dt/exception.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/translate_index.hpp>
#include <hbrs/mpl/detail/copy_matrix.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/fwd/dt/matrix_size.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <boost/hana/type.hpp>
#include <El.hpp>
#include <type_traits>
#include <initializer_list>

BOOST_HANA_NAMESPACE_BEGIN

template <typename Ring>
struct tag_of<El::Matrix<Ring>> {
	using type = ext::El::Matrix_tag;
};

template <>
struct make_impl<ext::El::Matrix_tag> {
	template <typename Ring>
	static constexpr El::Matrix<Ring>
	apply(basic_type<Ring>, hbrs::mpl::matrix_size<El::Int, El::Int> sz) {
		return {sz.m(), sz.n()};
	}
	
	template <
		typename T,
		typename M,
		typename N,
		hbrs::mpl::storage_order Order,
		typename std::enable_if_t<
			std::is_convertible<M,El::Int>::value && std::is_convertible<N,El::Int>::value
		>* = nullptr
	>
	static constexpr auto
	apply(
		hbrs::mpl::sm<hbrs::mpl::rtsav<T>, hbrs::mpl::matrix_size<M, N>, Order> const& b
	) {
		return hbrs::mpl::detail::copy_matrix(b, El::Matrix<std::remove_cv_t<T>>{(El::Int)b.size().m(), (El::Int)b.size().n()});
	}
	
	template <
		typename T,
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
		hbrs::mpl::sm<hbrs::mpl::ctsav<T, Length>, hbrs::mpl::matrix_size<M, N>, Order> const& b
	) {
		return hbrs::mpl::detail::copy_matrix(b, El::Matrix<std::remove_cv_t<T>>{(El::Int)b.size().m(), (El::Int)b.size().n()});
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
		return hbrs::mpl::detail::copy_matrix(
			data,
			hbrs::mpl::storage_order_c<Order>,
			El::Matrix<std::remove_const_t<Ring>> {sz.m(), sz.n()}
		);
	}
};

BOOST_HANA_NAMESPACE_END

#endif // !ELEMENTAL_DT_MATRIX_HPP
