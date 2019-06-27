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

#ifndef HBRS_MPL_FN_INSERT_IMPL_STD_HPP
#define HBRS_MPL_FN_INSERT_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <boost/assert.hpp>
#include <map>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
	
template<
	typename M,
	typename Pair,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<M>, hana::ext::std::map_tag >::value &&
		std::is_same< hana::tag_of_t<Pair>, hana::ext::std::pair_tag >::value &&
		decltype(insert_impl_std_map::args_ok(std::declval<M>(), std::declval<Pair>())){}
	>*
>
constexpr decltype(auto)
insert_impl_std_map::operator()(M && m, Pair && p) const {
	auto r = m.emplace(
		std::make_pair(
			//TODO: Correct?!
			hana::first(HBRS_MPL_FWD(p)), 
			hana::second(HBRS_MPL_FWD(p))
		)
	);
	BOOST_ASSERT(r.second == true);
	return HBRS_MPL_FWD(m);
}

template<
	typename M,
	typename Pair,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<M>, hana::ext::std::map_tag >::value && 
		std::is_same< hana::tag_of_t<Pair>, hana::ext::std::pair_tag >::value &&
		decltype(insert_impl_std_map::args_ok(std::declval<M>(), std::declval<Pair>())){}
	>*
>
constexpr auto
insert_impl_std_map::operator()(M const& m, Pair && p) const {
	M m_{m};
	return operator()(std::move(m_), HBRS_MPL_FWD(p));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_INSERT_IMPL_STD_HPP
