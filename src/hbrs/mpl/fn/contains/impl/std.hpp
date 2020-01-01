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

#ifndef HBRS_MPL_FN_CONTAINS_IMPL_STD_HPP
#define HBRS_MPL_FN_CONTAINS_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/contains.hpp>
#include <boost/mpl/if.hpp>

#include <vector>
#include <tuple>
#include <numeric>
#include <algorithm>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename S,
	typename E,
	typename std::enable_if_t<
		boost::mpl::if_c<
			std::is_same_v< hana::tag_of_t<S>, hana::ext::std::vector_tag>,
			std::is_convertible<E&&, typename std::remove_reference_t<S>::value_type>,
			std::false_type
		>::type::value
		//TODO: Check if 'e' is comparable?
	>*
>
constexpr decltype(auto)
contains_impl_std_vector::operator()(S && s, E && e) const {
	return std::find(
		s.begin(),
		s.end(),
		HBRS_MPL_FWD(e)
	) != s.end();
}

template<
	typename S, 
	typename E, 
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value
	>*
>
constexpr decltype(auto)
contains_impl_std_tuple::operator()(S && s, E && e) const {
	return hana::contains(HBRS_MPL_FWD(s), HBRS_MPL_FWD(e));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_CONTAINS_IMPL_STD_HPP
