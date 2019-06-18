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

#ifndef HBRS_MPL_FN_POWER_IMPL_STD_HPP
#define HBRS_MPL_FN_POWER_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template<
	typename T,
	typename std::enable_if_t< 
		std::is_arithmetic<T>::value
	>*
>
constexpr decltype(auto)
power_impl_std_arithmetic::operator()(T base, T exp) const {
	return std::pow(base, exp);
}

template<typename T, typename U>
constexpr decltype(auto)
power_impl_std_complex::operator()(std::complex<T> const& x, std::complex<U> const& y) const {
	return std::pow(x, y);
}

template<typename T, typename U>
constexpr decltype(auto)
power_impl_std_complex::operator()(std::complex<T> const& x, U const& y) const {
	return std::pow(x, y);
}

template<typename T, typename U>
constexpr decltype(auto)
power_impl_std_complex::operator()(T const& x, std::complex<U> const& y) const {
	return std::pow(x, y);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_POWER_IMPL_STD_HPP
