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

#ifndef HBRS_MPL_DETAIL_IS_BRACES_CONSTRUCTIBLE_HPP
#define HBRS_MPL_DETAIL_IS_BRACES_CONSTRUCTIBLE_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/mpl/vector.hpp>
#include <type_traits>
#include <boost/predef.h>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#if BOOST_COMP_GNUC

/* NOTE: GCC up to at least version 8 does not allow to brace initialize references like 
 *       'decltype(int&{std::declval<int&>()})' while Clang accepts this code snippet.
 */

template<
	typename T,
	typename std::enable_if_t<
		std::is_lvalue_reference<T>::value
	>* = nullptr
>
constexpr void gcc_workaround_braces_initialize_refs(T& t);

template<typename T>
constexpr void gcc_workaround_braces_initialize_refs(T const& t);
	
template<
	typename T,
	typename std::enable_if_t<
		!std::is_lvalue_reference<T>::value
	>* = nullptr
>
constexpr T&& gcc_workaround_braces_initialize_refs(T&& t);

/* Src.: https://stackoverflow.com/a/20885988/6490710 */
template<class T, typename... Args>
decltype(void( gcc_workaround_braces_initialize_refs<T>({std::declval<Args>()...})), std::true_type())
test_is_braces_constructible(int);

template<class T, typename... Args>
std::false_type
test_is_braces_constructible(...);
//////////

/* Src.: https://stackoverflow.com/a/29251402/6490710 */
template< typename T, typename Identity, typename = std::void_t<>>
struct is_braces_constructible_impl : std::false_type {};

template< typename T, typename ...Args >
struct is_braces_constructible_impl<
	T,
	std::tuple< Args... >,
	std::void_t< decltype(gcc_workaround_braces_initialize_refs<T>({std::declval< Args >()...}))>
> : std::true_type {};
//////////

#else // !BOOST_COMP_GNUC

/* Src.: https://stackoverflow.com/a/20885988/6490710 */
template<class T, typename... Args>
decltype(void(T{std::declval<Args>()...}), std::true_type())
test_is_braces_constructible(int);

template<class T, typename... Args>
std::false_type
test_is_braces_constructible(...);
//////////


/* Src.: https://stackoverflow.com/a/29251402/6490710 */
template< typename T, typename Identity, typename = std::void_t<>>
struct is_braces_constructible_impl : std::false_type {};

template< typename T, typename ...Args >
struct is_braces_constructible_impl<
	T,
	std::tuple< Args... >,
	std::void_t< decltype(T{std::declval< Args >()...}) >
> : std::true_type {};
//////////

#endif // !BOOST_COMP_GNUC

template<class T, typename... Args>
struct is_braces_constructible {
// 	using type = decltype( test_is_braces_constructible<T, Args...>(0) );
	using type = is_braces_constructible_impl< T, std::tuple< Args... > >;
	static inline constexpr bool value = type::value;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_IS_BRACES_CONSTRUCTIBLE_HPP
