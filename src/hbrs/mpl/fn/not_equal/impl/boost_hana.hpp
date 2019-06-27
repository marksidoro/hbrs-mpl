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

#ifndef HBRS_MPL_FN_NOT_EQUAL_IMPL_BOOST_HANA_HPP
#define HBRS_MPL_FN_NOT_EQUAL_IMPL_BOOST_HANA_HPP

#include "../fwd/boost_hana.hpp"

#include <hbrs/mpl/detail/operators/impl/boost_hana.hpp>
#include <hbrs/mpl/fn/not.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <boost/hana/not_equal.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

HBRS_MPL_DEFINE_BOOST_HANA_INTEGRAL_CONSTANT_OPERATOR_IMPL_ARITY2(not_equal, HBRS_MPL_OPERATOR_NOT_EQUAL, not_equal)

template<
	typename PairL,
	typename PairR,
	typename std::enable_if_t<
		//TODO: Replace by compare types of concept products, like in boost/hana/equal.hpp under "Comparable for Products"
		std::is_same< hana::tag_of_t<PairL>, hana::pair_tag >::value &&
		std::is_same< hana::tag_of_t<PairR>, hana::pair_tag >::value
	>*
>
constexpr decltype(auto)
not_equal_impl_hana_pair::operator()(PairL const& l, PairR const& r) const {
	return not_(equal(l,r));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_NOT_EQUAL_IMPL_BOOST_HANA_HPP

