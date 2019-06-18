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

#ifndef HBRS_MPL_FN_CONTAINS_IMPL_BOOST_HANA_HPP
#define HBRS_MPL_FN_CONTAINS_IMPL_BOOST_HANA_HPP

#include "../fwd/boost_hana.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename S, 
	typename E, 
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<S>, hana::tuple_tag >::value &&
		hana::Comparable<E&&>::value
	>*
>
constexpr decltype(auto)
contains_impl_hana_tuple::operator()(S && s, E && e) const {
	return hana::contains(HBRS_MPL_FWD(s), HBRS_MPL_FWD(e));
}


/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_CONTAINS_IMPL_BOOST_HANA_HPP
