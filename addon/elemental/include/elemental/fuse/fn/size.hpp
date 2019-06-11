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

#ifndef ELEMENTAL_FUSE_FN_SIZE_HPP
#define ELEMENTAL_FUSE_FN_SIZE_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/function_object.hpp>

#include <elemental/fwd/dt/matrix.hpp>
#include <elemental/fwd/dt/dist_matrix.hpp>
#include <elemental/fwd/dt/vector.hpp>
#include <elemental/fwd/dt/dist_vector.hpp>

#include <boost/hana/tuple.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;

namespace detail {

HBRS_MPL_DEF_FO_TRY_METHOD(size_impl_matrix, matrix_tag, size)
HBRS_MPL_DEF_FO_TRY_METHOD(size_impl_dist_matrix, dist_matrix_tag, size)

HBRS_MPL_DEF_FO_TRY_METHOD(length_impl_column_vector, column_vector_tag, length)
HBRS_MPL_DEF_FO_TRY_METHOD(length_impl_row_vector, row_vector_tag, length)

HBRS_MPL_DEF_FO_TRY_METHOD(length_impl_dist_column_vector, dist_column_vector_tag, length)
HBRS_MPL_DEF_FO_TRY_METHOD(length_impl_dist_row_vector, dist_row_vector_tag, length)

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_SIZE_IMPLS boost::hana::make_tuple(                                                          \
		elemental::detail::length_impl_column_vector{},                                                                \
		elemental::detail::length_impl_row_vector{},                                                                   \
		elemental::detail::length_impl_dist_column_vector{},                                                           \
		elemental::detail::length_impl_dist_row_vector{},                                                              \
		elemental::detail::size_impl_matrix{},                                                                         \
		elemental::detail::size_impl_dist_matrix{}                                                                     \
	)

#endif // !ELEMENTAL_FUSE_FN_SIZE_HPP
