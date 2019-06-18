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

#ifndef HBRS_MPL_FN_ZIP_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_ZIP_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/zas.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <
	typename Matrix,
	typename Integer,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
		std::is_lvalue_reference<Matrix>::value
	>*
>
constexpr auto
zip_impl_smc_el_matrix_integer_range::operator()(smc<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
	return make_zas(a, b);
}

template <
	typename Matrix,
	typename Integer,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
		!std::is_lvalue_reference<Matrix>::value
	>*
>
constexpr auto
zip_impl_smc_el_matrix_integer_range::operator()(smc<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
	typedef decltype(a.at(0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	el_matrix<_Ring_> c{a.length(), 1};
	for(El::Int i = 0; i < a.length(); ++i) {
		c.at({i, 0}) = a.at(i);
	}
	
	return make_zas(make_smc(std::move(c), 0), b);
}

template <
	typename Matrix,
	typename Integer,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
		std::is_lvalue_reference<Matrix>::value
	>*
>
constexpr auto
zip_impl_smr_el_matrix_integer_range::operator()(smr<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
	return make_zas(a, b);
}

template <
	typename Matrix,
	typename Integer,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
		!std::is_lvalue_reference<Matrix>::value
	>*
>
constexpr auto
zip_impl_smr_el_matrix_integer_range::operator()(smr<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
	typedef decltype(a.at(0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	el_matrix<_Ring_> c{1, a.length()};
	for(El::Int i = 0; i < a.length(); ++i) {
		c.at({0, i}) = a.at(i);
	}
	
	return make_zas(make_smr(std::move(c), 0), b);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_ZIP_IMPL_ELEMENTAL_HPP
