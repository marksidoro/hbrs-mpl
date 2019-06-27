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

#ifndef HBRS_MPL_FN_AT_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_AT_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/detail/function_object.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/assert.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_el_matrix, el_matrix_tag, at)

struct at_impl_el_matrix_smr {
	template <
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(Matrix && m, El::Int i) const;
};

HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_el_column_vector, el_column_vector_tag, at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_el_row_vector, el_row_vector_tag, at)

#else
struct at_impl_el_matrix{};
struct at_impl_el_matrix_smr{};
struct at_impl_el_column_vector{};
struct at_impl_el_row_vector{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_AT_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                        \
		hbrs::mpl::detail::at_impl_el_column_vector{},                                                                 \
		hbrs::mpl::detail::at_impl_el_row_vector{},                                                                    \
		hbrs::mpl::detail::at_impl_el_matrix{},                                                                        \
		hbrs::mpl::detail::at_impl_el_matrix_smr{}                                                                     \
	)

#endif // !HBRS_MPL_FN_AT_FWD_ELEMENTAL_HPP
