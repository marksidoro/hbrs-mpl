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

#ifndef HBRS_MPL_FUSE_BOOST_HANA_DETAIL_OPERATORS_HPP
#define HBRS_MPL_FUSE_BOOST_HANA_DETAIL_OPERATORS_HPP

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
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <hbrs/mpl/detail/has_operator.hpp>
#include <type_traits>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/concept/integral_constant.hpp>

#include <boost/hana/plus.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/mult.hpp>
#include <boost/hana/div.hpp>
#include <boost/hana/mod.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/greater.hpp>
#include <boost/hana/less_equal.hpp>
#include <boost/hana/greater_equal.hpp>
#include <boost/hana/not.hpp>
#include <boost/hana/and.hpp>
#include <boost/hana/or.hpp>

/* NOTE: Handles std::integral_constant and hana::integral_constant ! */


/* NOTE: We cannot use is_core_applicable_trait for operands with different types, because static_assert's might be
 * triggered in default impls of e.g. hana::equals or hana::less for hana::size_ and hana::int_!
 */

//TODO: Implement operators for comparable types? Or is this covered by implicit conversion to std::integral_constant 
//      and the corresponding equality comparison functions?

#define _BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1(op_name, op_sign, op_hana)                                  \
	struct op_name ## _impl_hana_ic {                                                                                  \
		template<                                                                                                      \
			typename T,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				hana::IntegralConstant<T>::value                                                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T && i) const {                                                                                     \
			return op_sign i;                                                                                          \
		}                                                                                                              \
	};

#define _BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(op_name, op_sign, op_hana)                                  \
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
		operator()(L &&, R &&) const {                                                                                 \
			constexpr auto l = std::decay_t<L>::value;                                                                 \
			constexpr auto r = std::decay_t<R>::value;                                                                 \
			return hana::integral_constant<std::decay_t<decltype(l op_sign r)>, (l op_sign r) >{};                     \
		}                                                                                                              \
		                                                                                                               \
		template<                                                                                                      \
			typename L,                                                                                                \
			typename R,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				!(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&                             \
				((hana::IntegralConstant<L>::value && std::is_integral<R>::value) ||                                   \
				(std::is_integral<L>::value && hana::IntegralConstant<R>::value)) &&                                   \
				std::is_same<                                                                                          \
					std::decay_t<decltype(std::decay_t<L>::value)>,                                                    \
					std::decay_t<decltype(std::decay_t<R>::value)>                                                     \
				>::value                                                                                               \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(L && l, R && r) const {                                                                             \
			return HBRS_MPL_FWD(l) op_sign HBRS_MPL_FWD(r);                                                            \
		}                                                                                                              \
	};

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(plus,               HBRS_MPL_OPERATOR_PLUS,             plus         )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(minus,              HBRS_MPL_OPERATOR_MINUS,            minus        )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(multiply,           HBRS_MPL_OPERATOR_MULTIPLY,         mult         )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(divide,             HBRS_MPL_OPERATOR_DIVIDE,           div          )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(modulo,             HBRS_MPL_OPERATOR_MODULO,           mod          )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(equal,              HBRS_MPL_OPERATOR_EQUAL,            equal        )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(not_equal,          HBRS_MPL_OPERATOR_NOT_EQUAL,        not_equal    )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(less,               HBRS_MPL_OPERATOR_LESS,             less         )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(greater,            HBRS_MPL_OPERATOR_GREATER,          greater      )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(less_equal,         HBRS_MPL_OPERATOR_LESS_EQUAL,       less_equal   )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(greater_equal,      HBRS_MPL_OPERATOR_GREATER_EQUAL,    greater_equal)
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1(not_,               HBRS_MPL_OPERATOR_NOT,              not          )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(and_,               HBRS_MPL_OPERATOR_AND,              and          )
_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(or_,                HBRS_MPL_OPERATOR_OR,               or           )

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#undef _BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY1
#undef _BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2

#endif // !HBRS_MPL_FUSE_BOOST_HANA_DETAIL_OPERATORS_HPP
