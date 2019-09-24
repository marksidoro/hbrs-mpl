/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_ABSOLUTE_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_ABSOLUTE_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/absolute.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <typename From, typename To>
static void
absolute_el_impl(From const& a, To & b) {
	for(El::Int j=0; j < a.Width(); ++j) {
		for(El::Int i=0; i < a.Height(); ++i) {
			*b.Buffer(i,j) = (*absolute)(*a.LockedBuffer(i,j));
		}
	}
}

template <
	typename Ring,
	typename std::enable_if_t<
		std::is_arithmetic< Ring >::value /* TODO: absolute() is not yet implemented for El::Complex<> */
	>*
>
auto
absolute_impl_el_matrix::operator()(el_matrix<Ring> const& a) const {
	el_matrix<std::remove_const_t<Ring>> b{ a.m(), a.n() };
	absolute_el_impl(a.data(), b.data());
	return b;
}

template <
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename std::enable_if_t<
		std::is_arithmetic< Ring >::value /* TODO: absolute() is not yet implemented for El::Complex<> */
	>*
>
auto
absolute_impl_el_dist_matrix::operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a) const {
	el_dist_matrix<std::remove_const_t<Ring>, Columnwise, Rowwise, Wrapping> b{ a.data().Grid(), a.m(), a.n() };
	absolute_el_impl(a.data().LockedMatrix(), b.data().Matrix());
	return b;
}
/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_ABSOLUTE_IMPL_ELEMENTAL_HPP
