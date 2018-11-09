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

#ifndef HBRS_MPL_FUSE_STD_FN_CONTAINS_HPP
#define HBRS_MPL_FUSE_STD_FN_CONTAINS_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <boost/mpl/if.hpp>
#include <vector>
#include <tuple>
#include <numeric>
#include <algorithm>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct contains_impl_std_vector {
	template<
		typename S,
		typename E,
		typename std::enable_if_t<
			boost::mpl::if_c<
				std::is_same< hana::tag_of_t<S>, hana::ext::std::vector_tag>::value && hana::Comparable<E>::value,
				std::is_convertible<E&&, typename std::remove_reference_t<S>::value_type>,
				std::false_type
			>::type::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S && s, E && e) const {
		return std::find(
			s.begin(), 
			s.end(), 
			HBRS_MPL_FWD(e)
		) != s.end();
	}
};

struct contains_impl_std_tuple {
	template<
		typename S, 
		typename E, 
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S && s, E && e) const {
		return hana::contains(HBRS_MPL_FWD(s), HBRS_MPL_FWD(e));
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_STD_FN_CONTAINS_IMPLS boost::hana::make_tuple(                                                   \
		hbrs::mpl::detail::contains_impl_std_vector{},                                                                 \
		hbrs::mpl::detail::contains_impl_std_tuple{}                                                                   \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_CONTAINS_HPP
