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

#ifndef HBRS_MPL_FUSE_STD_FN_WHERE_HPP
#define HBRS_MPL_FUSE_STD_FN_WHERE_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/ext/boost/hana/ext/std/map.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#include <map>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct where_impl_std_map {
	template<
		typename M,
		typename K,
		typename std::enable_if_t< 
			boost::mpl::if_c<
				std::is_same< hana::tag_of_t<M>, hana::ext::std::map_tag >::value,
				std::is_convertible<K&&, typename std::remove_reference_t<M>::key_type>,
				std::false_type
			>::type::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(M && m, K && k) const {
		return HBRS_MPL_FWD(m).at(HBRS_MPL_FWD(k));
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_STD_FN_WHERE_IMPLS boost::hana::make_tuple(                                                      \
		hbrs::mpl::detail::where_impl_std_map{}                                                                        \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_WHERE_HPP
