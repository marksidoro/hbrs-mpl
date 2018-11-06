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

#pragma once

#ifndef HBRS_MPL_FUSE_BOOST_HANA_FN_EQUAL_HPP
#define HBRS_MPL_FUSE_BOOST_HANA_FN_EQUAL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/fuse/boost/hana/detail/operators.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/pair.hpp>
#include <hbrs/mpl/fn/and.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/first.hpp>
#include <hbrs/mpl/fn/second.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct equal_impl_hana_pair {
	template<
		typename PairL,
		typename PairR,
		typename std::enable_if_t<
			//TODO: Replace by compare types of concept products, like in boost/hana/equal.hpp under "Comparable for Products"
			std::is_same< hana::tag_of_t<PairL>, hana::pair_tag >::value &&
			std::is_same< hana::tag_of_t<PairR>, hana::pair_tag >::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(PairL const& l, PairR const& r) const {
		return (*and_)(equal(first(l), first(r)), equal(second(l), second(r)));
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_BOOST_HANA_FN_EQUAL_IMPLS boost::hana::make_tuple(                                      \
		hbrs::mpl::detail::equal_impl_hana_pair{},                                                                     \
		hbrs::mpl::detail::equal_impl_hana_ic{}                                                                        \
	)

#endif // !HBRS_MPL_FUSE_BOOST_HANA_FN_EQUAL_HPP
