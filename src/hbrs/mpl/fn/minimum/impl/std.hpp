/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_MINIMUM_IMPL_STD_HPP
#define HBRS_MPL_FN_MINIMUM_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/fn/less.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename LHS,
	typename RHS,
	typename std::enable_if_t<
		//TODO: Not strict enough?
		std::is_arithmetic_v<std::decay_t<LHS>> && std::is_arithmetic_v<std::decay_t<RHS>>
	>*
>
decltype(auto)
minimum_impl_std_arithmetic::operator()(LHS && lhs, RHS && rhs) const {
	return lhs < rhs ? HBRS_MPL_FWD(lhs) : HBRS_MPL_FWD(rhs);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_MINIMUM_IMPL_STD_HPP
