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

#ifndef HBRS_MPL_DETAIL_TYPE_OF_HPP
#define HBRS_MPL_DETAIL_TYPE_OF_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/type.hpp>
#include <tuple>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct type_of_impl {
	template <typename T>
	constexpr hana::basic_type<T&&>
	operator()(T&&) const {
		return {};
	}
};

constexpr type_of_impl type_of{};

//TODO: Better name? as_type?

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_TYPE_OF_HPP
