/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FUSE_HBRS_MPL_FN_LAST_HPP
#define HBRS_MPL_FUSE_HBRS_MPL_FN_LAST_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/range/fwd.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct last_impl_range {
	template<
		typename Range,
		typename std::enable_if_t< std::is_same< hana::tag_of_t<Range>, range_tag >::value >* = nullptr
	>
	constexpr decltype(auto)
	operator()(Range && r) const {
		return HBRS_MPL_FWD(r).last();
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_LAST_IMPLS_HBRS_MPL boost::hana::make_tuple(                                                  \
		hbrs::mpl::detail::last_impl_range{}                                                                           \
	)

#endif // !HBRS_MPL_FUSE_HBRS_MPL_FN_LAST_HPP
