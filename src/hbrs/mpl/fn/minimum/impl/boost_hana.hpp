/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_MINIMUM_IMPL_BOOST_HANA_HPP
#define HBRS_MPL_FN_MINIMUM_IMPL_BOOST_HANA_HPP

#include "../fwd/boost_hana.hpp"

#include <boost/hana/less.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template<
	typename L,
	typename R,
	typename std::enable_if_t<
		(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&
		std::is_same<hana::tag_of_t<L>, hana::tag_of_t<R>>::value
	>*
>
constexpr decltype(auto)
minimum_impl_hana_ic::operator()(L &&, R &&) const {
	constexpr auto l = std::decay_t<L>::value;
	constexpr auto r = std::decay_t<R>::value;
	return hana::integral_constant<std::decay_t<decltype(l<r ? l : r)>, (l<r ? l : r) >{};
}

template<
	typename L,
	typename R,
	typename std::enable_if_t<
		!(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&
		(hana::IntegralConstant<L>::value && std::is_integral<R>::value) &&
		std::is_same_v<std::decay_t<decltype(std::decay_t<L>::value)>, R>
	>*
>
constexpr decltype(auto)
minimum_impl_hana_ic::operator()(L const& l, R const& r) const {
	return l<r ? l : r;
}

template<
	typename L,
	typename R,
	typename std::enable_if_t<
		!(hana::IntegralConstant<L>::value && hana::IntegralConstant<R>::value) &&
		(std::is_integral<L>::value && hana::IntegralConstant<R>::value) &&
		std::is_same_v<L, std::decay_t<decltype(std::decay_t<R>::value)>>
	>*
>
constexpr decltype(auto)
minimum_impl_hana_ic::operator()(L const& l, R const& r) const {
	return l<r ? l : r;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_MINIMUM_IMPL_BOOST_HANA_HPP

