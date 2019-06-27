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

#ifndef HBRS_MPL_DETAIL_OPERATORS_IMPL_BOOST_HANA_HPP
#define HBRS_MPL_DETAIL_OPERATORS_IMPL_BOOST_HANA_HPP

#include "../fwd/boost_hana.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>

/* NOTE: Handles std::integral_constant and hana::integral_constant ! */


/* NOTE: We cannot use is_core_applicable_trait for operands with different types, because static_assert's might be
 * triggered in default impls of e.g. hana::equals or hana::less for hana::size_ and hana::int_!
 */

//TODO: Implement operators for comparable types? Or is this covered by implicit conversion to std::integral_constant 
//      and the corresponding equality comparison functions?

#define HBRS_MPL_DEFINE_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1(op_name, op_sign, op_hana)                   \
	template<                                                                                                          \
		typename T,                                                                                                    \
		typename std::enable_if_t<                                                                                     \
			hana::IntegralConstant<T>::value                                                                           \
		>*                                                                                                             \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	op_name ## _impl_hana_ic::operator()(T && i) const {                                                               \
		return op_sign i;                                                                                              \
	}

#define HBRS_MPL_DEFINE_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(op_name, op_sign, op_hana)                   \
	template<                                                                                                          \
		typename L,                                                                                                    \
		typename R,                                                                                                    \
		typename std::enable_if_t<                                                                                     \
			(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&                                  \
			std::is_same<hana::tag_of_t<L>, hana::tag_of_t<R>>::value                                                  \
		>*                                                                                                             \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	op_name ## _impl_hana_ic::operator()(L &&, R &&) const {                                                           \
		constexpr auto l = std::decay_t<L>::value;                                                                     \
		constexpr auto r = std::decay_t<R>::value;                                                                     \
		return hana::integral_constant<std::decay_t<decltype(l op_sign r)>, (l op_sign r) >{};                         \
	}                                                                                                                  \
	                                                                                                                   \
	template<                                                                                                          \
		typename L,                                                                                                    \
		typename R,                                                                                                    \
		typename std::enable_if_t<                                                                                     \
			!(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&                                 \
			(hana::IntegralConstant<L>::value && std::is_integral<R>::value) &&                                        \
			std::is_same_v<std::decay_t<decltype(std::decay_t<L>::value)>, R>                                          \
		>*                                                                                                             \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	op_name ## _impl_hana_ic::operator()(L const& l, R const& r) const {                                               \
		return l op_sign r;                                                                                            \
	}                                                                                                                  \
	                                                                                                                   \
	template<                                                                                                          \
		typename L,                                                                                                    \
		typename R,                                                                                                    \
		typename std::enable_if_t<                                                                                     \
			!(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&                                 \
			(std::is_integral<L>::value && hana::IntegralConstant<R>::value) &&                                        \
			std::is_same_v<L, std::decay_t<decltype(std::decay_t<R>::value)>>                                          \
		>*                                                                                                             \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	op_name ## _impl_hana_ic::operator()(L const& l, R const& r) const {                                               \
		return l op_sign r;                                                                                            \
	}

#endif // !HBRS_MPL_DETAIL_OPERATORS_IMPL_BOOST_HANA_HPP
