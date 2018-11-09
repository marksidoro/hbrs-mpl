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

#ifndef HBRS_MPL_FUSE_STD_FN_POWER_HPP
#define HBRS_MPL_FUSE_STD_FN_POWER_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/tuple.hpp>
#include <cmath>
#include <type_traits>
#include <complex>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

//TODO: Add more impls, see https://en.cppreference.com/w/cpp/numeric/math/pow
struct power_impl_std_arithmetic {
	template<
		typename T,
		typename std::enable_if_t< 
			std::is_arithmetic<T>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(T base, T exp) const {
		return std::pow(base, exp);
	}
};

struct power_impl_std_complex {
	template<typename T, typename U>
	constexpr decltype(auto)
	operator()(std::complex<T> const& x, std::complex<U> const& y) const {
		return std::pow(x, y);
	}
	
	template<typename T, typename U>
	constexpr decltype(auto)
	operator()(std::complex<T> const& x, U const& y) const {
		return std::pow(x, y);
	}
	
	template<typename T, typename U>
	constexpr decltype(auto)
	operator()(T const& x, std::complex<U> const& y) const {
		return std::pow(x, y);
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_STD_FN_POWER_IMPLS boost::hana::make_tuple(                                                      \
		hbrs::mpl::detail::power_impl_std_arithmetic{},                                                                \
		hbrs::mpl::detail::power_impl_std_complex{}                                                                        \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_POWER_HPP
