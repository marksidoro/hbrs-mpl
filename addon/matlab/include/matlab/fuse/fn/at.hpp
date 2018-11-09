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

#pragma once

#ifndef MATLAB_FUSE_FN_AT_HPP
#define MATLAB_FUSE_FN_AT_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <matlab/fwd/dt/matrix.hpp>
#include <matlab/fwd/dt/vector.hpp>
#include <hbrs/mpl/detail/function_object.hpp>
#include <hbrs/mpl/fwd/dt/matrix_index.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;
namespace detail {

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
	operator()(emxArray_real_T & a, mpl::matrix_index<int,int> i) const;

	boolean_T & 
	operator()(emxArray_boolean_T & a, mpl::matrix_index<int,int> i) const;

	real_T const& 
	operator()(emxArray_real_T const& a, mpl::matrix_index<int,int> i) const;
	
	boolean_T const& 
	operator()(emxArray_boolean_T const& a, mpl::matrix_index<int,int> i) const;
};

HBRS_MPL_DEF_FO_TRY_METHOD(at_impl_column_vector, matlab::column_vector_tag, at)
HBRS_MPL_DEF_FO_TRY_METHOD(at_impl_row_vector, matlab::row_vector_tag, at)
HBRS_MPL_DEF_FO_TRY_METHOD(at_impl_matrix, matlab::matrix_tag, at)

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_AT_IMPLS boost::hana::make_tuple(                                                               \
		matlab::detail::at_impl_emxArray{},                                                                            \
		matlab::detail::at_impl_column_vector{},                                                                       \
		matlab::detail::at_impl_row_vector{},                                                                          \
		matlab::detail::at_impl_matrix{}                                                                               \
	)

#endif // !MATLAB_FUSE_FN_AT_HPP
