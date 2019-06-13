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

#include <hbrs/mpl/fn/divide.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

//TODO: Add divide_impl_vector_Scalar and divide_impl_dist_vector_Scalar impls!

template auto divide_impl_matrix_scalar::operator()(matrix<float>              , float               const&) const;
template auto divide_impl_matrix_scalar::operator()(matrix<El::Complex<float>> , El::Complex<float>  const&) const;
template auto divide_impl_matrix_scalar::operator()(matrix<double>             , double              const&) const;
template auto divide_impl_matrix_scalar::operator()(matrix<El::Complex<double>>, El::Complex<double> const&) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END