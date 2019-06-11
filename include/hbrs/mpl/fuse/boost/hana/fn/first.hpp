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

#ifndef HBRS_MPL_FUSE_BOOST_HANA_FN_FIRST_HPP
#define HBRS_MPL_FUSE_BOOST_HANA_FN_FIRST_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/pair.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
	
struct first_impl_hana_pair {
	template<
		typename Pair,
		typename std::enable_if_t< std::is_same< hana::tag_of_t<Pair>, hana::pair_tag >::value >* = nullptr
	>
	constexpr decltype(auto)
	operator()(Pair && p) const {
		return hana::first(HBRS_MPL_FWD(p));
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_BOOST_HANA_FN_FIRST_IMPLS boost::hana::make_tuple(                                               \
		hbrs::mpl::detail::first_impl_hana_pair{}                                                                      \
	)

#endif // !HBRS_MPL_FUSE_BOOST_HANA_FN_FIRST_HPP
