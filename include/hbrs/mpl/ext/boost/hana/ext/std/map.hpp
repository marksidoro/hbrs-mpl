/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_EXT_BOOST_HANA_EXT_STD_MAP_HPP
#define HBRS_MPL_EXT_BOOST_HANA_EXT_STD_MAP_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <map>

BOOST_HANA_NAMESPACE_BEGIN

namespace ext { namespace std { struct map_tag; }}

template <class Key, class T, class Compare, class Allocator>
struct tag_of<std::map<Key, T, Compare, Allocator>> {
	using type = ext::std::map_tag;
};

BOOST_HANA_NAMESPACE_END

#endif // !HBRS_MPL_EXT_BOOST_HANA_EXT_STD_MAP_HPP
