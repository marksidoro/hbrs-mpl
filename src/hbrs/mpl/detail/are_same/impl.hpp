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

#ifndef HBRS_MPL_DETAIL_ARE_SAME_IMPL_HPP
#define HBRS_MPL_DETAIL_ARE_SAME_IMPL_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/and.hpp>
#include <tuple>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <typename... Xs>
struct are_same;

template <typename... Xs>
constexpr bool are_same_v = are_same<Xs...>::value;

template<typename... Xs>
struct are_same : std::conditional<
	hana::and_(
		hana::bool_c<std::is_same<
			Xs, typename std::tuple_element<0, std::tuple<Xs...>>::type
		>::value>...
	),
	std::true_type,
	std::false_type
>::type {};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_ARE_SAME_IMPL_HPP
