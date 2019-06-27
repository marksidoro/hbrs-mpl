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

#ifndef HBRS_MPL_DETAIL_OPERATORS_FWD_BOOST_HANA_HPP
#define HBRS_MPL_DETAIL_OPERATORS_FWD_BOOST_HANA_HPP

#include "operators.hpp"

#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/concept/integral_constant.hpp>

/* NOTE: Handles std::integral_constant and hana::integral_constant ! */

/* NOTE: We cannot use is_core_applicable_trait for operands with different types, because static_assert's might be
 * triggered in default impls of e.g. hana::equals or hana::less for hana::size_ and hana::int_!
 */

//TODO: Implement operators for comparable types? Or is this covered by implicit conversion to std::integral_constant 
//      and the corresponding equality comparison functions?

#define HBRS_MPL_DECLARE_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1(op_name, op_sign, op_hana)                  \
	struct op_name ## _impl_hana_ic {                                                                                  \
		template<                                                                                                      \
			typename T,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				hana::IntegralConstant<T>::value                                                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T && i) const;                                                                                      \
	};

#define HBRS_MPL_DECLARE_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(op_name, op_sign, op_hana)                  \
	struct op_name ## _impl_hana_ic {                                                                                  \
		template<                                                                                                      \
			typename L,                                                                                                \
			typename R,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&                              \
				std::is_same<hana::tag_of_t<L>, hana::tag_of_t<R>>::value                                              \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(L &&, R &&) const;                                                                                  \
		                                                                                                               \
		template<                                                                                                      \
			typename L,                                                                                                \
			typename R,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				!(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&                             \
				(hana::IntegralConstant<L>::value && std::is_integral<R>::value) &&                                    \
				std::is_same_v<std::decay_t<decltype(std::decay_t<L>::value)>, R>                                      \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(L const& l, R const& r) const;                                                                      \
		                                                                                                               \
		template<                                                                                                      \
			typename L,                                                                                                \
			typename R,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				!(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&                             \
				(std::is_integral<L>::value && hana::IntegralConstant<R>::value) &&                                    \
				std::is_same_v<L, std::decay_t<decltype(std::decay_t<R>::value)>>                                      \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(L const& l, R const& r) const;                                                                      \
	};

#endif // !HBRS_MPL_DETAIL_OPERATORS_FWD_BOOST_HANA_HPP
