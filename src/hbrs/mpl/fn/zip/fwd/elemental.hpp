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

#ifndef HBRS_MPL_FN_ZIP_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_ZIP_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/smc/fwd.hpp>
	#include <hbrs/mpl/dt/smr/fwd.hpp>
	#include <boost/range/irange.hpp>
	#include <El.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct zip_impl_smc_el_matrix_integer_range {
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(smc<Matrix, El::Int> a, boost::integer_range<Integer> b) const;
	
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
			!std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(smc<Matrix, El::Int> a, boost::integer_range<Integer> b) const;
};

struct zip_impl_smr_el_matrix_integer_range {
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(smr<Matrix, El::Int> a, boost::integer_range<Integer> b) const;
	
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value &&
			!std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(smr<Matrix, El::Int> a, boost::integer_range<Integer> b) const;
};

#else
struct zip_impl_smc_el_matrix_integer_range {};
struct zip_impl_smr_el_matrix_integer_range {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_ZIP_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                       \
		hbrs::mpl::detail::zip_impl_smc_el_matrix_integer_range{},                                                     \
		hbrs::mpl::detail::zip_impl_smr_el_matrix_integer_range{}                                                      \
	)

#endif // !HBRS_MPL_FN_ZIP_FWD_ELEMENTAL_HPP
