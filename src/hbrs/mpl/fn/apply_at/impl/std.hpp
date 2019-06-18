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

#ifndef HBRS_MPL_FN_APPLY_AT_IMPL_STD_HPP
#define HBRS_MPL_FN_APPLY_AT_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/apply_at.hpp>
#include <hbrs/mpl/detail/is_homogenous_container.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/mpl/if.hpp>
#include <type_traits>
#include <array>
#include <tuple>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename F,
	typename S,
	typename std::enable_if_t<
		boost::mpl::if_c<
			std::is_same< hana::tag_of_t<S>, hana::ext::std::array_tag >::value,
			std::is_invocable<F&&, typename std::remove_reference_t<S>::value_type>,
			std::false_type
		>::type::value
	>*
>
constexpr decltype(auto)
apply_at_impl_std_array_integral::operator()(F && f, S && s, std::size_t w) const {
	return hana::apply(HBRS_MPL_FWD(f), HBRS_MPL_FWD(s)[w]);
}

template <
	typename F,
	typename S,
	typename Where,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag >::value &&
		hana::IntegralConstant<Where>::value
	>*
>
constexpr decltype(auto)
apply_at_impl_std_tuple_ic::operator()(F && f, S && s, Where && w) const {
	return hana::apply(HBRS_MPL_FWD(f), hana::at(HBRS_MPL_FWD(s), HBRS_MPL_FWD(w)));
}

template <
	typename F,
	typename S,
	typename std::enable_if_t<
		boost::mpl::if_c<
			std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag >::value,
			is_applicable_at<F&&,S&&>,
			std::false_type
		>::type::value
	>*
>
constexpr decltype(auto)
apply_at_impl_std_tuple_integral::operator()(F && f, S && s, std::size_t w) const {
	return detail::apply_at(HBRS_MPL_FWD(f), HBRS_MPL_FWD(s), w);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_APPLY_AT_IMPL_STD_HPP
