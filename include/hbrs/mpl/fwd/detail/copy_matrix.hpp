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

#ifndef HBRS_MPL_FWD_DETAIL_COPY_MATRIX_HPP
#define HBRS_MPL_FWD_DETAIL_COPY_MATRIX_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <initializer_list>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <typename From, typename To>
constexpr decltype(auto)
copy_matrix(From const& from, To && to);

template <typename From, typename To>
constexpr decltype(auto)
copy_matrix(From const& from, To & to);

template <typename From, storage_order Order, typename To>
constexpr decltype(auto)
copy_matrix(std::initializer_list<From> from, storage_order_<Order>, To && to);

template <typename From, storage_order Order, typename To>
constexpr decltype(auto)
copy_matrix(std::initializer_list<From> from, storage_order_<Order>, To & to);

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FWD_DETAIL_COPY_MATRIX_HPP
