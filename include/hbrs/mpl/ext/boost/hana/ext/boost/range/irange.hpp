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

#ifndef HBRS_MPL_EXT_BOOST_HANA_EXT_BOOST_RANGE_IRANGE_HPP
#define HBRS_MPL_EXT_BOOST_HANA_EXT_BOOST_RANGE_IRANGE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/fwd/core/tag_of.hpp>
#include <boost/range/irange.hpp>

BOOST_HANA_NAMESPACE_BEGIN

namespace ext { namespace boost { struct integer_range_tag; }}

template <typename Integer>
struct tag_of<boost::integer_range<Integer>> {
	using type = ext::boost::integer_range_tag;
};

BOOST_HANA_NAMESPACE_END

#endif // !HBRS_MPL_EXT_BOOST_HANA_EXT_BOOST_RANGE_IRANGE_HPP
