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

#ifndef HBRS_MPL_FN_INDICES_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_INDICES_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <boost/range/irange.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <
	typename Matrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
	>*
>
constexpr auto
indices_impl_smc_el_matrix::operator()(smc<Matrix, El::Int> const& a) const {
	return boost::irange(El::Int{0}, (*size)(a));
}

template <
	typename Matrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
	>*
>
constexpr auto
indices_impl_smr_el_matrix::operator()(smr<Matrix, El::Int> const& a) const {
	return boost::irange(El::Int{0}, (*size)(a));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_INDICES_IMPL_ELEMENTAL_HPP
