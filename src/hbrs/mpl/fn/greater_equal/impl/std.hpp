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

#ifndef HBRS_MPL_FUSE_STD_FN_GREATER_EQUAL_HPP
#define HBRS_MPL_FUSE_STD_FN_GREATER_EQUAL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/fuse/std/detail/operators.hpp>
#include <boost/hana/tuple.hpp>
#include <hbrs/mpl/fn/less.hpp>
#include <array>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

/* compare operators for std::array are constexpr since C++20 only!
 * Ref.: https://en.cppreference.com/w/cpp/container/array/operator_cmp
 */
struct greater_equal_impl_std_array {
	template <class T, std::size_t N>
	constexpr bool 
	operator()(std::array<T, N> const& lhs, std::array<T, N> const& rhs) {
		return !(*less)(lhs, rhs);
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_GREATER_EQUAL_IMPLS_STD boost::hana::make_tuple(                                              \
		hbrs::mpl::detail::greater_equal_impl_std_array{},                                                             \
		hbrs::mpl::detail::greater_equal_impl_std_ic{},                                                                \
		hbrs::mpl::detail::greater_equal_impl_std_op{},                                                                \
		hbrs::mpl::detail::greater_equal_impl_lhs_is_braces_constructible{},                                           \
		hbrs::mpl::detail::greater_equal_impl_rhs_is_braces_constructible{},                                           \
		hbrs::mpl::detail::greater_equal_impl_numeric_cast{},                                                          \
		hbrs::mpl::detail::greater_equal_impl_op{}                                                                     \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_GREATER_EQUAL_HPP
