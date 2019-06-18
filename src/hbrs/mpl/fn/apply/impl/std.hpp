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

#ifndef HBRS_MPL_FN_APPLY_IMPL_STD_HPP
#define HBRS_MPL_FN_APPLY_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_applicable.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename F,
	typename Args,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Args>, hana::ext::std::tuple_tag >::value &&
		is_applicable_trait<F&&, Args&&>::value
	>*
>
constexpr decltype(auto)
apply_impl_std_tuple_unpack::operator()(F && f, Args && args) const {
	return hana::unpack(HBRS_MPL_FWD(args), HBRS_MPL_FWD(f));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_APPLY_IMPL_STD_HPP
