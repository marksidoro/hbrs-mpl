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

#include "elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template auto expand_impl_smr_el_matrix::operator()(
	smr<el_matrix<float>, El::Int> const&,
	matrix_size<El::Int, El::Int> const&) const;

template auto expand_impl_smr_el_matrix::operator()(
	smr<el_matrix<El::Complex<float>>, El::Int> const&,
	matrix_size<El::Int, El::Int> const&) const;

template auto expand_impl_smr_el_matrix::operator()(
	smr<el_matrix<double>, El::Int> const&,
	matrix_size<El::Int, El::Int> const&) const;

template auto expand_impl_smr_el_matrix::operator()(
	smr<el_matrix<El::Complex<double>>, El::Int> const&,
	matrix_size<El::Int, El::Int> const&) const;

//TODO: Add el_dist_row_vector, el_row_vector, el_dist_column_vector and el_column_vector impls!

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
