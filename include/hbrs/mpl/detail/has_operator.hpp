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

#ifndef HBRS_MPL_DETAIL_HAS_OPERATOR_HPP
#define HBRS_MPL_DETAIL_HAS_OPERATOR_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/type.hpp>

#include <hbrs/mpl/fwd/fn/preincrement.hpp>
#include <hbrs/mpl/fwd/fn/predecrement.hpp>
#include <hbrs/mpl/fwd/fn/postincrement.hpp>
#include <hbrs/mpl/fwd/fn/postdecrement.hpp>
#include <hbrs/mpl/fwd/fn/plus.hpp>
#include <hbrs/mpl/fwd/fn/minus.hpp>
#include <hbrs/mpl/fwd/fn/multiply.hpp>
#include <hbrs/mpl/fwd/fn/divide.hpp>
#include <hbrs/mpl/fwd/fn/modulo.hpp>
#include <hbrs/mpl/fwd/fn/equal.hpp>
#include <hbrs/mpl/fwd/fn/not_equal.hpp>
#include <hbrs/mpl/fwd/fn/less.hpp>
#include <hbrs/mpl/fwd/fn/greater.hpp>
#include <hbrs/mpl/fwd/fn/less_equal.hpp>
#include <hbrs/mpl/fwd/fn/greater_equal.hpp>
#include <hbrs/mpl/fwd/fn/not.hpp>
#include <hbrs/mpl/fwd/fn/and.hpp>
#include <hbrs/mpl/fwd/fn/or.hpp>

#define _HAS_OPERATOR_IMPL_ARITY1(op_name, op_sign)                                                                    \
	_HAS_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)

#define _HAS_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)                                                                \
	struct has_ ## op_name ## _operator_test {                                                                         \
		template<typename A>                                                                                           \
		constexpr auto                                                                                                 \
		operator()(A&& a)                                                                                              \
		-> decltype(                                                                                                   \
			(void)                                                                                                     \
			op_sign(std::declval<A>())                                                                                 \
		)                                                                                                              \
		{}                                                                                                             \
	};                                                                                                                 \
	constexpr auto has_## op_name ## _operator = hana::is_valid(has_ ## op_name ## _operator_test{});

#define _HAS_OPERATOR_IMPL_ARITY1_POST(op_name, op_sign)                                                               \
	struct has_ ## op_name ## _operator_test {                                                                         \
		template<typename A>                                                                                           \
		constexpr auto                                                                                                 \
		operator()(A&& a)                                                                                              \
		-> decltype(                                                                                                   \
			(void)                                                                                                     \
			(std::declval<A>())op_sign                                                                                 \
		)                                                                                                              \
		{}                                                                                                             \
	};                                                                                                                 \
	constexpr auto has_## op_name ## _operator = hana::is_valid(has_ ## op_name ## _operator_test{});                  \

#define _HAS_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                                                    \
	struct has_ ## op_name ## _operator_test {                                                                         \
		template<typename A, typename B>                                                                               \
		constexpr auto                                                                                                 \
		operator()(A&& a, B&& b)                                                                                       \
		-> decltype(                                                                                                   \
			(void)                                                                                                     \
			(HBRS_MPL_FWD(a) op_sign HBRS_MPL_FWD(b))                                                                  \
		)                                                                                                              \
		{}                                                                                                             \
	};                                                                                                                 \
	constexpr auto has_## op_name ## _operator = hana::is_valid(has_ ## op_name ## _operator_test{});                  \

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

/* NOTE: has_*_operator perform a flat test only, but no deep test! They only test whether the called operator has right
 *       signature for the given operands, but it does not test whether functions, that are invoked by operator, can be 
 *       applied to the given operands, too. This e.g. means that has_*_operator always returns true for unconstrained 
 *       template functions!
 */

_HAS_OPERATOR_IMPL_ARITY1_PRE( preincrement,        HBRS_MPL_OPERATOR_PREINCREMENT)
_HAS_OPERATOR_IMPL_ARITY1_PRE( predecrement,        HBRS_MPL_OPERATOR_PREDECREMENT)
_HAS_OPERATOR_IMPL_ARITY1_POST(postincrement,       HBRS_MPL_OPERATOR_POSTINCREMENT)
_HAS_OPERATOR_IMPL_ARITY1_POST(postdecrement,       HBRS_MPL_OPERATOR_POSTDECREMENT)
_HAS_OPERATOR_IMPL_ARITY2(     plus,                HBRS_MPL_OPERATOR_PLUS)
_HAS_OPERATOR_IMPL_ARITY2(     minus,               HBRS_MPL_OPERATOR_MINUS)
_HAS_OPERATOR_IMPL_ARITY2(     multiply,            HBRS_MPL_OPERATOR_MULTIPLY)
_HAS_OPERATOR_IMPL_ARITY2(     divide,              HBRS_MPL_OPERATOR_DIVIDE)
_HAS_OPERATOR_IMPL_ARITY2(     modulo,              HBRS_MPL_OPERATOR_MODULO)
_HAS_OPERATOR_IMPL_ARITY2(     equal,               HBRS_MPL_OPERATOR_EQUAL)
_HAS_OPERATOR_IMPL_ARITY2(     not_equal,           HBRS_MPL_OPERATOR_NOT_EQUAL)
_HAS_OPERATOR_IMPL_ARITY2(     less,                HBRS_MPL_OPERATOR_LESS)
_HAS_OPERATOR_IMPL_ARITY2(     greater,             HBRS_MPL_OPERATOR_GREATER)
_HAS_OPERATOR_IMPL_ARITY2(     less_equal,          HBRS_MPL_OPERATOR_LESS_EQUAL)
_HAS_OPERATOR_IMPL_ARITY2(     greater_equal,       HBRS_MPL_OPERATOR_GREATER_EQUAL)
_HAS_OPERATOR_IMPL_ARITY1(     not,                 HBRS_MPL_OPERATOR_NOT)
_HAS_OPERATOR_IMPL_ARITY2(     and,                 HBRS_MPL_OPERATOR_AND)
_HAS_OPERATOR_IMPL_ARITY2(     or,                  HBRS_MPL_OPERATOR_OR)

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#undef _HAS_OPERATOR_IMPL_ARITY1
#undef _HAS_OPERATOR_IMPL_ARITY1_PRE
#undef _HAS_OPERATOR_IMPL_ARITY1_POST
#undef _HAS_OPERATOR_IMPL_ARITY2

#endif // !HBRS_MPL_DETAIL_HAS_OPERATOR_HPP
