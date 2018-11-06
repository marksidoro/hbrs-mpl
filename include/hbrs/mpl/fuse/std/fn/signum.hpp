/* Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FUSE_STD_FN_SIGNUM_HPP
#define HBRS_MPL_FUSE_STD_FN_SIGNUM_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/core/evaluate.hpp>

#include <hbrs/mpl/fn/transform.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/ext/std/tuple.hpp>

#include <array>
#include <vector>
#include <tuple>
#include <numeric>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct signum_impl_std_arithmetic {
	/* Src.: https://stackoverflow.com/a/4609795/6490710 */
	template<
		typename T,
		typename std::enable_if_t<
			std::is_arithmetic<T>::value && std::is_signed<T>::value
		>* = nullptr
	>
	constexpr int
	operator()(T x) const {
		return (T(0) < x) - (x < T(0));
	}
	
	template<
		typename T,
		typename std::enable_if_t<
			std::is_arithmetic<T>::value && !std::is_signed<T>::value
		>* = nullptr
	>
	constexpr int
	operator()(T x) const {
		return T(0) < x;
	}
};

struct signum_impl_std_ic {
	template<
		typename T, T x,
		typename std::enable_if_t<
			std::is_arithmetic<T>::value && std::is_signed<T>::value
		>* = nullptr
	>
	constexpr std::integral_constant<int, ((T(0) < x) - (x < T(0)))>
	operator()(std::integral_constant<T, x>) const {
		return {};
	}
	
	template<
		typename T, T x,
		typename std::enable_if_t<
			std::is_arithmetic<T>::value && !std::is_signed<T>::value
		>* = nullptr
	>
	constexpr std::integral_constant<int, (T(0) < x)>
	operator()(std::integral_constant<T, x>) const {
		return {};
	}
};

struct signum_impl_std_array_vector {
	template<
		typename S,
		typename std::enable_if_t<
			(
				std::is_same< hana::tag_of_t<S>, hana::ext::std::array_tag>::value ||
				std::is_same< hana::tag_of_t<S>, hana::ext::std::vector_tag>::value
			) &&
			std::is_arithmetic<
				typename std::remove_reference_t<S>::value_type
			>::value
			//TODO: Replace by check "signum(S::value_type) != error"
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S&& s) const {
		return (*transform)(HBRS_MPL_FWD(s), signum);
	}
};

struct signum_impl_std_tuple {
	template<
		typename S,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S && s) const {
		return (*transform)(HBRS_MPL_FWD(s), signum);
	}
};
/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_STD_FN_SIGNUM_IMPLS boost::hana::make_tuple(                                                     \
		hbrs::mpl::detail::signum_impl_std_arithmetic{},                                                               \
		hbrs::mpl::detail::signum_impl_std_ic{},                                                                       \
		hbrs::mpl::detail::signum_impl_std_array_vector{},                                                             \
		hbrs::mpl::detail::signum_impl_std_tuple{}                                                                     \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_SIGNUM_HPP
