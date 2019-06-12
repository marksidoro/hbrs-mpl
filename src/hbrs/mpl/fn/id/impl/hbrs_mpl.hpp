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

#ifndef HBRS_MPL_FUSE_HBRS_MPL_FN_ID_HPP
#define HBRS_MPL_FUSE_HBRS_MPL_FN_ID_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct id_impl {
	template <typename T>
	constexpr decltype(auto)
	operator()(T && t) const {
		return HBRS_MPL_FWD(t);
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_HBRS_MPL_FN_ID_IMPLS boost::hana::make_tuple(                                                    \
		hbrs::mpl::detail::id_impl{}                                                                                   \
	)                                                                                                                  \

#endif // !HBRS_MPL_FUSE_HBRS_MPL_FN_ID_HPP
