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

#ifndef HBRS_MPL_FN_AT_FWD_MATLAB_HPP
#define HBRS_MPL_FN_AT_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/ml_vector/fwd.hpp>
#endif

#include <hbrs/mpl/dt/matrix_index/fwd.hpp>
#include <hbrs/mpl/detail/function_object.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

struct at_impl_emxArray {
	real_T & 
	operator()(emxArray_real_T & v, int d0) const;

	boolean_T & 
	operator()(emxArray_boolean_T & v, int d0) const;

	real_T const& 
	operator()(emxArray_real_T const& v, int d0) const;

	boolean_T const& 
	operator()(emxArray_boolean_T const& v, int d0) const;

	real_T & 
	operator()(emxArray_real_T & a, matrix_index<int,int> i) const;

	boolean_T & 
	operator()(emxArray_boolean_T & a, matrix_index<int,int> i) const;

	real_T const& 
	operator()(emxArray_real_T const& a, matrix_index<int,int> i) const;
	
	boolean_T const& 
	operator()(emxArray_boolean_T const& a, matrix_index<int,int> i) const;
};

HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_ml_column_vector, ml_column_vector_tag, at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_ml_row_vector, ml_row_vector_tag, at)
HBRS_MPL_DEC_FO_TRY_METHOD(at_impl_ml_matrix, ml_matrix_tag, at)

#else
struct at_impl_emxArray{};
struct at_impl_ml_column_vector{};
struct at_impl_ml_row_vector{};
struct at_impl_ml_matrix{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_AT_IMPLS_MATLAB boost::hana::make_tuple(                                                           \
		hbrs::mpl::detail::at_impl_emxArray{},                                                                         \
		hbrs::mpl::detail::at_impl_ml_column_vector{},                                                                 \
		hbrs::mpl::detail::at_impl_ml_row_vector{},                                                                    \
		hbrs::mpl::detail::at_impl_ml_matrix{}                                                                         \
	)

#endif // !HBRS_MPL_FN_AT_FWD_MATLAB_HPP
