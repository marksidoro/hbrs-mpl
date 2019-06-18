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

#ifndef HBRS_MPL_FN_SIZE_IMPL_BOOST_RANGE_HPP
#define HBRS_MPL_FN_SIZE_IMPL_BOOST_RANGE_HPP

#include "../fwd/boost_range.hpp"

#include <boost/range/size.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template<typename Integer>
constexpr decltype(auto)
size_impl_range_integer_range::operator()(boost::integer_range<Integer> const& s) const {
	return s.size();
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_SIZE_IMPL_BOOST_RANGE_HPP
