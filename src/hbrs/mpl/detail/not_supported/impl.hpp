/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DETAIL_NOT_SUPPORTED_IMPL_HPP
#define HBRS_MPL_DETAIL_NOT_SUPPORTED_IMPL_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/not.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

struct not_supported{};

template<typename T>
inline constexpr auto is_not_supported_v =
	hana::bool_c<
		std::is_same_v<std::decay_t<T>, not_supported>
	>;

struct is_not_supported_t {
	template<typename T>
	constexpr decltype(auto)
	operator()(T&&) const {
		return is_not_supported_v<T>;
	}
};

constexpr is_not_supported_t is_not_supported{};

template<typename T>
inline constexpr auto is_supported_v = !is_not_supported_v<T>;

struct is_supported_t {
	template<typename T>
	constexpr decltype(auto)
	operator()(T&&) const {
		return is_supported_v<T>;
	}
};

constexpr is_supported_t is_supported{};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_NOT_SUPPORTED_IMPL_HPP
