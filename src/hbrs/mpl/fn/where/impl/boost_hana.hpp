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

#ifndef HBRS_MPL_FUSE_BOOST_HANA_FN_WHERE_HPP
#define HBRS_MPL_FUSE_BOOST_HANA_FN_WHERE_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_core_applicable.hpp>
#include <boost/hana/map.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/concept/comparable.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct where_impl_hana_map {
	template <
		typename M,
		typename K, 
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<M>, hana::map_tag >::value &&
			hana::Comparable<K&&>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(M && m, K && k) const {
		return hana::find(HBRS_MPL_FWD(m), HBRS_MPL_FWD(k));
	}
};
	
/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_BOOST_HANA_FN_WHERE_IMPLS boost::hana::make_tuple(                                               \
		hbrs::mpl::detail::where_impl_hana_map{}                                                                       \
	)

#endif // !HBRS_MPL_FUSE_BOOST_HANA_FN_WHERE_HPP
