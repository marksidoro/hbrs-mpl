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

#ifndef HBRS_MPL_FN_SIZE_FWD_MATLAB_HPP
#define HBRS_MPL_FN_SIZE_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/detail/function_object.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/ml_vector/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB
HBRS_MPL_DEC_FO_TRY_METHOD(length_impl_ml_column_vector, ml_column_vector_tag, length)
HBRS_MPL_DEC_FO_TRY_METHOD(length_impl_ml_row_vector, ml_row_vector_tag, length)
HBRS_MPL_DEC_FO_TRY_METHOD(size_impl_ml_matrix, ml_matrix_tag, size)
#else
struct length_impl_ml_column_vector {};
struct length_impl_ml_row_vector {};
struct size_impl_ml_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SIZE_IMPLS_MATLAB boost::hana::make_tuple(                                                         \
		hbrs::mpl::detail::size_impl_ml_matrix{},                                                                      \
		hbrs::mpl::detail::length_impl_ml_column_vector{},                                                             \
		hbrs::mpl::detail::length_impl_ml_row_vector{}                                                                 \
	)

#endif // !HBRS_MPL_FN_SIZE_FWD_MATLAB_HPP
