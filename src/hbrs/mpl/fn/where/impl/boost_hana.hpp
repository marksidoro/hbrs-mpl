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

#ifndef HBRS_MPL_FN_WHERE_IMPL_BOOST_HANA_HPP
#define HBRS_MPL_FN_WHERE_IMPL_BOOST_HANA_HPP

#include "../fwd/boost_hana.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/find.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename M,
	typename K, 
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<M>, hana::map_tag >::value &&
		hana::Comparable<K&&>::value
	>*
>
constexpr decltype(auto)
where_impl_hana_map::operator()(M && m, K && k) const {
	return hana::find(HBRS_MPL_FWD(m), HBRS_MPL_FWD(k));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_WHERE_IMPL_BOOST_HANA_HPP
