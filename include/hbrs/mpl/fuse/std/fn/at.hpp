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

#ifndef HBRS_MPL_FUSE_STD_FN_AT_HPP
#define HBRS_MPL_FUSE_STD_FN_AT_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/is_homogenous_container.hpp>
#include <hbrs/mpl/detail/apply_at.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/functional/id.hpp>
#include <array>
#include <vector>
#include <tuple>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct at_impl_std_array {
	template<
		typename S,
		typename Where,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<S>, hana::ext::std::array_tag>::value &&
			hana::IntegralConstant<Where>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S && s, Where&&) const {
		return HBRS_MPL_FWD(s)[std::remove_reference_t<Where>::value];
	}
	
	template<
		typename S,
		typename std::enable_if_t< std::is_same< hana::tag_of_t<S>, hana::ext::std::array_tag>::value >* = nullptr
	>
	constexpr decltype(auto)
	operator()(S&& s, std::size_t w) const {
		return HBRS_MPL_FWD(s)[w];
	}
};

struct at_impl_std_vector {
	template<
		typename S,
		typename Where,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<S>, hana::ext::std::vector_tag>::value &&
			hana::IntegralConstant<Where>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S && s, Where&&) const {
		return HBRS_MPL_FWD(s)[std::remove_reference_t<Where>::value];
	}
	
	template<
		typename S,
		typename std::enable_if_t< std::is_same< hana::tag_of_t<S>, hana::ext::std::vector_tag>::value >* = nullptr
	>
	constexpr decltype(auto)
	operator()(S && s, std::size_t w) const {
		return HBRS_MPL_FWD(s)[w];
	}
};

struct at_impl_std_tuple {
	template<
		typename S,
		typename Where,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value &&
			hana::IntegralConstant<Where>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S&& s, Where &&) const {
		return std::get<std::remove_reference_t<Where>::value>(HBRS_MPL_FWD(s));
	}
};

struct at_impl_std_tuple_hmg_integral {
	
	template<
		typename S,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value &&
			is_homogenous_container_v<S>
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S&& s, std::size_t w) const {
		return detail::apply_at(hana::id, HBRS_MPL_FWD(s), w);
	}
	
// 	template<
// 		typename S,
// 		typename Where,
// 		typename std::enable_if_t< 
// 			std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value &&
// 			is_homogenous_container_v<S> &&
// 			std::is_integral<Where>::value
// 		>* = nullptr
// 	>
// 	constexpr decltype(auto)
// 	operator()(S&& s, Where && w) const {
// 		return detail::apply_at(hana::id, HBRS_MPL_FWD(s), HBRS_MPL_FWD(w));
// 	}
};
/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_STD_FN_AT_IMPLS boost::hana::make_tuple(                                                         \
		hbrs::mpl::detail::at_impl_std_array{},                                                                        \
		hbrs::mpl::detail::at_impl_std_vector{},                                                                       \
		hbrs::mpl::detail::at_impl_std_tuple{},                                                                        \
		hbrs::mpl::detail::at_impl_std_tuple_hmg_integral{}                                                            \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_AT_HPP
