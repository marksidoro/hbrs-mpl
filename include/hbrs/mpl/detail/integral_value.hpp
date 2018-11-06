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

#ifndef HBRS_MPL_DETAIL_INTEGRAL_VALUE_HPP
#define HBRS_MPL_DETAIL_INTEGRAL_VALUE_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/integral_constant.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename T,
	typename std::enable_if_t<
		hana::IntegralConstant<std::decay_t<T>>::value || std::is_integral<std::decay_t<T>>::value
	>* = nullptr
>
inline static constexpr decltype(auto)
integral_value(T && t){
	typedef std::decay_t<T> _T_;
	if constexpr (hana::IntegralConstant<_T_>::value) {
		return _T_::value;
	} else {
		return std::forward<T>(t);
	}
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_INTEGRAL_VALUE_HPP
