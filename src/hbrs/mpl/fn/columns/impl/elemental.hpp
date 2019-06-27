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

#ifndef HBRS_MPL_FN_COLUMNS_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_COLUMNS_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/config.hpp>

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>

#include <hbrs/mpl/fn/columns/fwd.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/expression.hpp>

#include <type_traits>
#include <vector>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <
	typename Matrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
	>*
>
constexpr auto
columns_impl_el_matrix_1::operator()(Matrix && a) const {
	return smcs<Matrix>{HBRS_MPL_FWD(a)};
}

template <typename Ring>
constexpr auto
columns_impl_el_matrix_2::operator()(el_matrix<Ring> const& a) const {
	typedef std::decay_t<Ring> _Ring_;
	auto a_sz = (*size)(a);
	auto a_n = (*n)(a_sz);
	
	std::vector<el_row_vector<_Ring_>> v;
	v.reserve(a_n);
	for(El::Int i = 0; i < a_n; ++i) {
		v.emplace_back( a.data()(El::ALL, i) );
	}
	return v;
}

template <
	typename DistMatrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value
	>*
>
constexpr auto
columns_impl_el_dist_matrix::operator()(DistMatrix && a) const {
	return make_expression(columns, std::tuple<decltype(a)>{HBRS_MPL_FWD(a)});
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_COLUMNS_IMPL_ELEMENTAL_HPP
