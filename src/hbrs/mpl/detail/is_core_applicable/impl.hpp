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

#ifndef HBRS_MPL_DETAIL_IS_CORE_APPLICABLE_HPP
#define HBRS_MPL_DETAIL_IS_CORE_APPLICABLE_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/type.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

/* NOTE: is_core_applicable performs a flat test only, but no deep test! It only tests whether the called type T has right
 *       signature for the given operands, but it does not test whether functions, that are invoked by T, can be applied
 *       to the given operands, too. This e.g. means that is_core_applicable always returns true for unconstrained template 
 *       functions!
 */

struct is_core_applicable_impl {
	template<typename T, typename... Operands>
	constexpr auto
	operator()(T&&, Operands&&... os) 
	-> decltype(
		(void)
		std::decay_t<T>::apply(
			std::declval<Operands>()...
		)
	)
	{}
};

constexpr auto is_core_applicable = hana::is_valid(is_core_applicable_impl{});

template<typename T, typename Operands, typename = void>
struct is_core_applicable_trait_impl : std::false_type {};

template<typename T, template <typename...> class Ctnr, typename... Operands>
struct is_core_applicable_trait_impl<
	T, Ctnr<Operands...>,
	decltype(
		(void)
		std::decay_t<T>::apply(
			std::declval<Operands>()...
		)
	) 
> : std::true_type {};

template<typename T, typename... Operands>
struct is_core_applicable_trait {
	using type = typename is_core_applicable_trait_impl<T, std::tuple<Operands...>>::type;
	static constexpr bool value = type::value;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_IS_CORE_APPLICABLE_HPP
