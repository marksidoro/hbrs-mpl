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

#ifndef HBRS_MPL_DETAIL_OPERATORS_FWD_STD_HPP
#define HBRS_MPL_DETAIL_OPERATORS_FWD_STD_HPP

#include "operators.hpp"

#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <hbrs/mpl/detail/has_operator.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <type_traits>
#include <complex>

/* TODO: Give higher priority to specialized impls by only enabling default operators if no other impls exist. 
 * This is important e.g. if you plus/minus/.. two compile-time integrals because those are implicitly convertible
 * to runtime-time integrals (std::integral_constant<std::size_t, ...> => std::size_t) which will delay a result
 * that could be computed at compile-time to runtime-time.
 * 
 * As an alternative approach here we enable runtime values only with is_arithmetic and thus workaround implicit 
 * conversions.
 */

#define HBRS_MPL_DECLARE_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1(op_name, op_sign)                                  \
	struct op_name ## _impl_std_ic {                                                                                   \
		template<                                                                                                      \
			typename T1, T1 v1                                                                                         \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(std::integral_constant<T1, v1>) const;                                                              \
	};

#define HBRS_MPL_DECLARE_STD_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                  \
	struct op_name ## _impl_std_ic {                                                                                   \
		template<                                                                                                      \
			typename T1, T1 v1,                                                                                        \
			typename T2, T2 v2                                                                                         \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(std::integral_constant<T1, v1>, std::integral_constant<T2, v2>) const;                              \
		                                                                                                               \
		template<                                                                                                      \
			typename T1, T1 v1,                                                                                        \
			typename T2,                                                                                               \
			typename std::enable_if_t<                                                                                 \
				std::is_integral<std::decay_t<T2>>::value || std::is_enum<std::decay_t<T2>>::value                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(std::integral_constant<T1, v1>, T2 v2) const;                                                       \
		                                                                                                               \
		template<                                                                                                      \
			typename T1,                                                                                               \
			typename T2, T2 v2,                                                                                        \
			typename std::enable_if_t<                                                                                 \
				std::is_integral<std::decay_t<T1>>::value || std::is_enum<std::decay_t<T1>>::value                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(T1 v1, std::integral_constant<T2, v2>) const;                                                       \
	};

#define HBRS_MPL_DECLARE_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1(op_name, op_sign)                                    \
	struct op_name ## _impl_std_op {                                                                                   \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value)                   \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const;                                                                                       \
	};

#define HBRS_MPL_DECLARE_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)                                \
	struct op_name ## _impl_std_op {                                                                                   \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value) &&                \
				std::is_lvalue_reference<A>::value                                                                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const;                                                                                       \
	};

#define HBRS_MPL_DECLARE_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY1_POST(op_name, op_sign)                               \
	struct op_name ## _impl_std_op {                                                                                   \
		template <                                                                                                     \
			typename A,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				(std::is_arithmetic<std::decay_t<A>>::value || std::is_enum<std::decay_t<A>>::value) &&                \
				std::is_lvalue_reference<A>::value                                                                     \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr auto                                                                                                 \
		operator()(A&& a) const;                                                                                       \
	};

#define HBRS_MPL_DECLARE_STD_ARITHMETIC_ENUM_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                    \
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
		operator()(A&& a, B&& b) const;                                                                                \
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
		operator()(A&& a, B&& b) const;                                                                                \
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
		operator()(LHS && lhs, RHS && rhs);                                                                            \
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
		operator()(LHS && lhs, RHS && rhs);                                                                            \
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
		operator()(LHS && lhs, RHS && rhs);                                                                            \
	};

#define HBRS_MPL_DECLARE_STD_COMPLEX_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                            \
	struct op_name ## _impl_std_complex_op {                                                                           \
		template <                                                                                                     \
			typename T,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				std::is_floating_point_v<T>                                                                            \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(std::complex<T> const& lhs, std::complex<T> const& rhs) const;                                      \
		                                                                                                               \
		template <                                                                                                     \
			typename T,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				std::is_floating_point_v<T>                                                                            \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(std::complex<T> const& lhs, T const& rhs) const;                                                    \
		                                                                                                               \
		template <                                                                                                     \
			typename T,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				std::is_floating_point_v<T>                                                                            \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T const& lhs, std::complex<T> const& rhs) const;                                                    \
	};

/* NOTE: Using free function operators are disabled because e.g. those operators are defined for 
 *       boost::hana::integral_constant but calling them using with invalid input in has_*_op function triggers 
 *       assertions, because boost hana concept checks fail.
 */
#define HBRS_MPL_DECLARE_FREEFUN_OPERATOR_IMPL_ARITY1(op_name, op_sign)                                                \
	HBRS_MPL_DECLARE_FREEFUN_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)

#define HBRS_MPL_DECLARE_FREEFUN_OPERATOR_IMPL_ARITY1_PRE(op_name, op_sign)                                            \
	struct op_name ## _impl_op {                                                                                       \
		/*template <                                                                                                   \
			typename A,                                                                                                \
			typename std::enable_if_t<decltype(                                                                        \
				detail::has_ ## op_name ## _operator(std::declval<A>())                                                \
			)::value>* = nullptr                                                                                       \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const;                                                                                       \
		*/                                                                                                             \
	};

#define HBRS_MPL_DECLARE_FREEFUN_OPERATOR_IMPL_ARITY1_POST(op_name, op_sign)                                           \
	struct op_name ## _impl_op {                                                                                       \
		/*template <                                                                                                   \
			typename A,                                                                                                \
			typename std::enable_if_t<decltype(                                                                        \
				detail::has_ ## op_name ## _operator(std::declval<A>())                                                \
			)::value>* = nullptr                                                                                       \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a) const;                                                                                       \
		*/                                                                                                             \
	};

/* NOTE: Dangerous, because free function operator does not necessarily perform conformance checks of invoked 
 *       functions. detail::has_*_operator might return falsely true because a free function is defined for 
 *       these types, but then compilation fail because e.g. a function invoked by that free function is not 
 *       compatible or because that free function triggers an assertion
 */
#define HBRS_MPL_DECLARE_FREEFUN_OPERATOR_IMPL_ARITY2(op_name, op_sign)                                                \
	struct op_name ## _impl_op {                                                                                       \
		/*template <                                                                                                   \
			typename A,                                                                                                \
			typename B,                                                                                                \
			typename std::enable_if_t<decltype(                                                                        \
				detail::has_ ## op_name ## _operator(std::declval<A>(), std::declval<B>())                             \
			)::value>* = nullptr                                                                                       \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(A&& a, B&& b) const;                                                                                \
		*/                                                                                                             \
	};

#endif // !HBRS_MPL_DETAIL_OPERATORS_FWD_STD_HPP
