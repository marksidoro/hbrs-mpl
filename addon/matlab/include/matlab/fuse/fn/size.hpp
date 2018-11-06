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

#ifndef MATLAB_FUSE_FN_SIZE_HPP
#define MATLAB_FUSE_FN_SIZE_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/function_object.hpp>

#include <matlab/fwd/dt/matrix.hpp>
#include <matlab/fwd/dt/vector.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

HBRS_MPL_DEF_FO_TRY_METHOD(length_impl_column_vector, matlab::column_vector_tag, length)
HBRS_MPL_DEF_FO_TRY_METHOD(length_impl_row_vector, matlab::row_vector_tag, length)
HBRS_MPL_DEF_FO_TRY_METHOD(size_impl_matrix, matlab::matrix_tag, size)

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_SIZE_IMPLS boost::hana::make_tuple(                                                             \
		matlab::detail::size_impl_matrix{},                                                                            \
		matlab::detail::length_impl_column_vector{},                                                                   \
		matlab::detail::length_impl_row_vector{}                                                                       \
	)

#endif // !MATLAB_FUSE_FN_SIZE_HPP
