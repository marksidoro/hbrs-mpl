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

#ifndef HBRS_MPL_FUSE_STD_DETAIL_OPERATORS_HPP
#define HBRS_MPL_FUSE_STD_DETAIL_OPERATORS_HPP

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

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <hbrs/mpl/core/evaluate.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

/* TODO: Give higher priority to specialized impls by only enabling default operators if no other impls exist. 
 * This is important e.g. if you plus/minus/.. two compile-time integrals because those are implicitly convertible
 * to runtime-time integrals (std::integral_constant<std::size_t, ...> => std::size_t) which will delay a result
 * that could be computed at compile-time to runtime-time.
 * 
 * As an alternative approach here we enable runtime values only with is_arithmetic and thus workaround implicit 
 * conversions.
 */

#define _STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1(op_name, op_sign)                                                  \
	struct op_name ## _impl_std_ic {                                                                                   \
		template<                                                                                                      \
			typename T1, T1 v1                                                                                         \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(std::integral_constant<T1, v1>) const {                                                             \
			return std::integral_constant<decltype(op_sign v1), op_sign v1>{};                                         \
		}                                                                                                              \
	};

#define _STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                                  \
	struct op_name ## _impl_std_ic {                                                                                   \
		template<                                                                                                      \
			typename T1, T1 v1,                                                                                        \
			typename T2, T2 v2                                                                                         \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(std::integral_constant<T1, v1>, std::integral_constant<T2, v2>) const {                             \
			return std::integral_constant<decltype(v1 op_sign v2), (v1 op_sign v2)>{};                                 \
		}                                                                                                              \
		                                                                                                               \
		template<                                                                                                      \
			typename T1, T1 v1,                                                                                        \
			typename T2,                                                                                               \
			typename std::enable_if_t<                                                                                 \
				std::is_integral<std::decay_t<T2>>::value || std::is_enum<std::decay_t<T2>>::value                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(std::integral_constant<T1, v1>, T2 v2) const {                                                      \
			return evaluate(op_name(v1, v2));                                                                          \
		}                                                                                                              \
		                                                                                                               \
		template<                                                                                                      \
			typename T1,                                                                                               \
			typename T2, T2 v2,                                                                                        \
			typename std::enable_if_t<                                                                                 \
				std::is_integral<std::decay_t<T1>>::value || std::is_enum<std::decay_t<T1>>::value                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(T1 v1, std::integral_constant<T2, v2>) const {                                                      \
			return evaluate(op_name(v1, v2));                                                                          \
		}                                                                                                              \
	};

#define _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1(op_name, op_sign)                                                    \
	struct op_name ## _impl_std_op {                                                                                   \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value)                   \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const {                                                                                      \
			return op_sign HBRS_MPL_FWD(a);                                                                            \
		}                                                                                                              \
	};

#define _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)                                                \
	struct op_name ## _impl_std_op {                                                                                   \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value) &&                \
				std::is_lvalue_reference<A>::value                                                                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const {                                                                                      \
			return op_sign HBRS_MPL_FWD(a);                                                                            \
		}                                                                                                              \
	};

#define _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_POST(op_name, op_sign)                                               \
	struct op_name ## _impl_std_op {                                                                                   \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value) &&                \
				std::is_lvalue_reference<A>::value                                                                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(A&& a) const {                                                                                      \
			return HBRS_MPL_FWD(a) op_sign;                                                                            \
		}                                                                                                              \
	};

#define _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                                    \
	struct op_name ## _impl_std_op {                                                                                   \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename B,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				std::is_same<std::decay_t<A>, std::decay_t<B>>::value &&                                               \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value)                   \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a, B&& b) const {                                                                               \
			return HBRS_MPL_FWD(a) op_sign HBRS_MPL_FWD(b);                                                            \
		}                                                                                                              \
		                                                                                                               \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename B,                                                                                                \
			typename std::enable_if_t<                                                                                 \
			    !std::is_same<std::decay_t<A>, std::decay_t<B>>::value &&                                              \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value) &&                \
				(std::is_arithmetic<std::decay_t<B>>::value || std::is_enum<std::decay_t<B>>::value) &&                \
				(std::is_signed<std::decay_t<A>>::value == std::is_signed<std::decay_t<B>>::value)                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a, B&& b) const {                                                                               \
			return HBRS_MPL_FWD(a) op_sign HBRS_MPL_FWD(b);                                                            \
		}                                                                                                              \
	};                                                                                                                 \
                                                                                                                       \
	struct op_name ## _impl_lhs_is_braces_constructible {                                                              \
		template <                                                                                                     \
			typename LHS,                                                                                              \
			typename RHS,                                                                                              \
			typename std::enable_if_t<                                                                                 \
				boost::mpl::if_c<                                                                                      \
				/* we do is_braces_constructible tests only for arithmetics and enums because is_braces_constructible  \
				 * might fail to compile for non-default constructors */                                               \
					!std::is_same<                                                                                     \
						std::decay_t<LHS>,                                                                             \
						std::decay_t<RHS>                                                                              \
					>::value &&                                                                                        \
					(std::is_arithmetic<std::decay_t<LHS>>::value || std::is_enum<std::decay_t<LHS>>::value) &&        \
					(std::is_arithmetic<std::decay_t<RHS>>::value || std::is_enum<std::decay_t<RHS>>::value),          \
					hbrs::mpl::detail::is_braces_constructible<                                                        \
						std::decay_t<LHS>,                                                                             \
						std::decay_t<RHS>                                                                              \
					>,                                                                                                 \
					std::false_type                                                                                    \
				>::type::value                                                                                         \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(LHS && lhs, RHS && rhs) {                                                                           \
			return HBRS_MPL_FWD(lhs) op_sign std::decay_t<LHS>{ HBRS_MPL_FWD(rhs) };                                   \
		}                                                                                                              \
	};                                                                                                                 \
                                                                                                                       \
	struct op_name ## _impl_rhs_is_braces_constructible {                                                              \
		template <                                                                                                     \
			typename LHS,                                                                                              \
			typename RHS,                                                                                              \
			typename std::enable_if_t<                                                                                 \
				/* we do is_braces_constructible tests only for arithmetics and enums because is_braces_constructible  \
				 * might fail to compile for non-default constructors */                                               \
				boost::mpl::if_c<                                                                                      \
					!std::is_same<                                                                                     \
						std::decay_t<LHS>,                                                                             \
						std::decay_t<RHS>                                                                              \
					>::value &&                                                                                        \
					(std::is_arithmetic<std::decay_t<LHS>>::value || std::is_enum<std::decay_t<LHS>>::value) &&        \
					(std::is_arithmetic<std::decay_t<RHS>>::value || std::is_enum<std::decay_t<RHS>>::value),          \
					hbrs::mpl::detail::is_braces_constructible<                                                        \
						std::decay_t<RHS>,                                                                             \
						std::decay_t<LHS>                                                                              \
					>,                                                                                                 \
					std::false_type                                                                                    \
				>::type::value                                                                                         \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(LHS && lhs, RHS && rhs) {                                                                           \
			return std::decay_t<RHS>{ HBRS_MPL_FWD(lhs) } op_sign HBRS_MPL_FWD(rhs);                                   \
		}                                                                                                              \
	};                                                                                                                 \
	                                                                                                                   \
	struct op_name ## _impl_numeric_cast {                                                                             \
		template <                                                                                                     \
			typename LHS,                                                                                              \
			typename RHS,                                                                                              \
			typename std::enable_if_t<                                                                                 \
				/* we do is_braces_constructible tests only for arithmetics and enums because is_braces_constructible  \
				 * might fail to compile for non-default constructors */                                               \
				boost::mpl::if_c<                                                                                      \
					!std::is_same< std::decay_t<LHS>, std::decay_t<RHS> >::value &&                                    \
					std::is_arithmetic<std::decay_t<LHS>>::value &&                                                    \
					std::is_arithmetic<std::decay_t<RHS>>::value,                                                      \
					boost::mpl::not_<                                                                                  \
						boost::mpl::or_<                                                                               \
							hbrs::mpl::detail::is_braces_constructible< std::decay_t<LHS>, std::decay_t<RHS> >,        \
							hbrs::mpl::detail::is_braces_constructible< std::decay_t<RHS>, std::decay_t<LHS> >         \
						>                                                                                              \
					>,                                                                                                 \
					std::false_type                                                                                    \
				>::type::value                                                                                         \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		[[deprecated("numeric_cast<> required, input arguments okay?")]]                                               \
		constexpr decltype(auto)                                                                                       \
		/* NOTE: boost::numeric_cast is not yet constexpr, see                                                         \
		 *       https://github.com/boostorg/numeric_conversion/issues/13                                              \
		 */                                                                                                            \
		operator()(LHS && lhs, RHS && rhs) {                                                                           \
			/* assumption: cast fails less often if we cast smaller number to larger numbers */                        \
			/*if constexpr (sizeof(std::remove_reference_t<LHS>) < sizeof(std::remove_reference_t<RHS>)) {             \
				return                                                                                                 \
					boost::numeric_cast<std::remove_reference_t<RHS>>(HBRS_MPL_FWD(lhs))                               \
					op_sign                                                                                            \
					HBRS_MPL_FWD(rhs);                                                                                 \
			} else {                                                                                                   \
				return                                                                                                 \
					HBRS_MPL_FWD(lhs)                                                                                  \
					op_sign                                                                                            \
					boost::numeric_cast<std::remove_reference_t<LHS>>(HBRS_MPL_FWD(rhs));                              \
			}*/                                                                                                        \
			/* TODO: This impl. or impl above? */                                                                      \
			return                                                                                                     \
				boost::numeric_cast<std::common_type_t<std::decay_t<LHS>, std::decay_t<RHS>>>(HBRS_MPL_FWD(lhs))       \
				op_sign                                                                                                \
				boost::numeric_cast<std::common_type_t<std::decay_t<LHS>, std::decay_t<RHS>>>(HBRS_MPL_FWD(rhs));      \
		}                                                                                                              \
	};

/* NOTE: Using free function operators are disabled because e.g. those operators are defined for 
 *       boost::hana::integral_constant but calling them using with invalid input in has_*_op function triggers 
 *       assertions, because boost hana concept checks fail.
 */
#define _FREEFUN_OPERATOR_IMPL_ARITY1(op_name, op_sign)                                                                \
	_FREEFUN_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)

#define _FREEFUN_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)                                                            \
	struct op_name ## _impl_op {                                                                                       \
		/*template <                                                                                                   \
			typename A,                                                                                                \
			typename std::enable_if_t<decltype(                                                                        \
				detail::has_ ## op_name ## _operator(std::declval<A>())                                                \
			)::value>* = nullptr                                                                                       \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const {                                                                                      \
			return operator op_sign(HBRS_MPL_FWD(a));                                                                  \
		}*/                                                                                                            \
	};

#define _FREEFUN_OPERATOR_IMPL_ARITY1_POST(op_name, op_sign)                                                           \
	struct op_name ## _impl_op {                                                                                       \
		/*template <                                                                                                   \
			typename A,                                                                                                \
			typename std::enable_if_t<decltype(                                                                        \
				detail::has_ ## op_name ## _operator(std::declval<A>())                                                \
			)::value>* = nullptr                                                                                       \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const {                                                                                      \
			return operator op_sign(HBRS_MPL_FWD(a), 0);                                                               \
		}*/                                                                                                            \
	};

/* NOTE: Dangerous, because free function operator does not necessarily perform conformance checks of invoked 
 *       functions. detail::has_*_operator might return falsely true because a free function is defined for 
 *       these types, but then compilation fail because e.g. a function invoked by that free function is not 
 *       compatible or because that free function triggers an assertion
 */
#define _FREEFUN_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                                                \
	struct op_name ## _impl_op {                                                                                       \
		/*template <                                                                                                   \
			typename A,                                                                                                \
			typename B,                                                                                                \
			typename std::enable_if_t<decltype(                                                                        \
				detail::has_ ## op_name ## _operator(std::declval<A>(), std::declval<B>())                             \
			)::value>* = nullptr                                                                                       \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a, B&& b) const {                                                                               \
			return operator op_sign(HBRS_MPL_FWD(a), HBRS_MPL_FWD(b));                                                 \
		}*/                                                                                                            \
	};

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(plus,                HBRS_MPL_OPERATOR_PLUS)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(minus,               HBRS_MPL_OPERATOR_MINUS)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(multiply,            HBRS_MPL_OPERATOR_MULTIPLY)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(divide,              HBRS_MPL_OPERATOR_DIVIDE)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(modulo,              HBRS_MPL_OPERATOR_MODULO)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(equal,               HBRS_MPL_OPERATOR_EQUAL)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(not_equal,           HBRS_MPL_OPERATOR_NOT_EQUAL)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(less,                HBRS_MPL_OPERATOR_LESS)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(greater,             HBRS_MPL_OPERATOR_GREATER)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(less_equal,          HBRS_MPL_OPERATOR_LESS_EQUAL)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(greater_equal,       HBRS_MPL_OPERATOR_GREATER_EQUAL)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1(not_,                HBRS_MPL_OPERATOR_NOT)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(and_,                HBRS_MPL_OPERATOR_AND)
_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(or_,                 HBRS_MPL_OPERATOR_OR)

_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_PRE( preincrement,     HBRS_MPL_OPERATOR_PREINCREMENT)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_PRE( predecrement,     HBRS_MPL_OPERATOR_PREDECREMENT)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_POST(postincrement,    HBRS_MPL_OPERATOR_POSTINCREMENT)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_POST(postdecrement,    HBRS_MPL_OPERATOR_POSTDECREMENT)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     plus,             HBRS_MPL_OPERATOR_PLUS)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     minus,            HBRS_MPL_OPERATOR_MINUS)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     multiply,         HBRS_MPL_OPERATOR_MULTIPLY)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     divide,           HBRS_MPL_OPERATOR_DIVIDE)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     modulo,           HBRS_MPL_OPERATOR_MODULO)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     equal,            HBRS_MPL_OPERATOR_EQUAL)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     not_equal,        HBRS_MPL_OPERATOR_NOT_EQUAL)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     less,             HBRS_MPL_OPERATOR_LESS)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     greater,          HBRS_MPL_OPERATOR_GREATER)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     less_equal,       HBRS_MPL_OPERATOR_LESS_EQUAL)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     greater_equal,    HBRS_MPL_OPERATOR_GREATER_EQUAL)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1(     not_,             HBRS_MPL_OPERATOR_NOT)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     and_,             HBRS_MPL_OPERATOR_AND)
_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(     or_,              HBRS_MPL_OPERATOR_OR)

_FREEFUN_OPERATOR_IMPL_ARITY1_PRE( preincrement,        HBRS_MPL_OPERATOR_PREINCREMENT)
_FREEFUN_OPERATOR_IMPL_ARITY1_PRE( predecrement,        HBRS_MPL_OPERATOR_PREDECREMENT)
_FREEFUN_OPERATOR_IMPL_ARITY1_POST(postincrement,       HBRS_MPL_OPERATOR_POSTINCREMENT)
_FREEFUN_OPERATOR_IMPL_ARITY1_POST(postdecrement,       HBRS_MPL_OPERATOR_POSTDECREMENT)
_FREEFUN_OPERATOR_IMPL_ARITY2(     plus,                HBRS_MPL_OPERATOR_PLUS)
_FREEFUN_OPERATOR_IMPL_ARITY2(     minus,               HBRS_MPL_OPERATOR_MINUS)
_FREEFUN_OPERATOR_IMPL_ARITY2(     multiply,            HBRS_MPL_OPERATOR_MULTIPLY)
_FREEFUN_OPERATOR_IMPL_ARITY2(     divide,              HBRS_MPL_OPERATOR_DIVIDE)
_FREEFUN_OPERATOR_IMPL_ARITY2(     modulo,              HBRS_MPL_OPERATOR_MODULO)
_FREEFUN_OPERATOR_IMPL_ARITY2(     equal,               HBRS_MPL_OPERATOR_EQUAL)
_FREEFUN_OPERATOR_IMPL_ARITY2(     not_equal,           HBRS_MPL_OPERATOR_NOT_EQUAL)
_FREEFUN_OPERATOR_IMPL_ARITY2(     less,                HBRS_MPL_OPERATOR_LESS)
_FREEFUN_OPERATOR_IMPL_ARITY2(     greater,             HBRS_MPL_OPERATOR_GREATER)
_FREEFUN_OPERATOR_IMPL_ARITY2(     less_equal,          HBRS_MPL_OPERATOR_LESS_EQUAL)
_FREEFUN_OPERATOR_IMPL_ARITY2(     greater_equal,       HBRS_MPL_OPERATOR_GREATER_EQUAL)
_FREEFUN_OPERATOR_IMPL_ARITY1(     not_,                HBRS_MPL_OPERATOR_NOT)
_FREEFUN_OPERATOR_IMPL_ARITY2(     and_,                HBRS_MPL_OPERATOR_AND)
_FREEFUN_OPERATOR_IMPL_ARITY2(     or_,                 HBRS_MPL_OPERATOR_OR)

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#undef _FREEFUN_OPERATOR_IMPL_ARITY1
#undef _FREEFUN_OPERATOR_IMPL_ARITY1_PRE
#undef _FREEFUN_OPERATOR_IMPL_ARITY1_POST
#undef _FREEFUN_OPERATOR_IMPL_ARITY2
#undef _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1
#undef _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_PRE
#undef _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_POST
#undef _STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2
#undef _STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1
#undef _STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2

#endif // !HBRS_MPL_FUSE_STD_DETAIL_OPERATORS_HPP
