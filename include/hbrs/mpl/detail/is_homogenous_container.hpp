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

#ifndef HBRS_MPL_DETAIL_IS_HOMOGENOUS_CONTAINER_HPP
#define HBRS_MPL_DETAIL_IS_HOMOGENOUS_CONTAINER_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/detail/are_same.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<typename>
struct is_homogenous_container_ : std::false_type {};

template<template<typename...> class Container, typename... Elements>
struct is_homogenous_container_<Container<Elements...>> : are_same<Elements...> { };

template<typename Container>
struct is_homogenous_container : is_homogenous_container_< std::decay_t<Container> > { };

template<typename T>
constexpr bool is_homogenous_container_v = is_homogenous_container<T>::value;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_IS_HOMOGENOUS_CONTAINER_HPP
