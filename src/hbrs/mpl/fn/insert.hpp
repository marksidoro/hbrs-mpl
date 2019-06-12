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

#ifndef HBRS_MPL_FN_INSERT_HPP
#define HBRS_MPL_FN_INSERT_HPP

#include <hbrs/mpl/fwd/fn/insert.hpp>
#include <hbrs/mpl/dt/function.hpp>

HBRS_MPL_NAMESPACE_BEGIN
HBRS_MPL_DEF_F2(insert, insert_t)
HBRS_MPL_NAMESPACE_END

#include <hbrs/mpl/fuse/fn/insert.hpp>
#include <hbrs/mpl/core/implementations_of.hpp>
HBRS_MPL_MAP_IMPLS(insert_t, HBRS_MPL_FUSE_FN_INSERT_IMPLS)

#endif // !HBRS_MPL_FN_INSERT_HPP
