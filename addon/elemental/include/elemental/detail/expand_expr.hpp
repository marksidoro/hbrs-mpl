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

#ifndef ELEMENTAL_DETAIL_EXPAND_EXPR_HPP
#define ELEMENTAL_DETAIL_EXPAND_EXPR_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <El.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

template<typename Expandable>
struct expand_expr {
	Expandable from;
	mpl::matrix_size<El::Int, El::Int> to_size;
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#endif // !ELEMENTAL_DETAIL_EXPAND_EXPR_HPP
