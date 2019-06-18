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

template auto plus_impl_el_matrix_el_matrix::operator()(el_matrix<float>,               el_matrix<float>               const&) const;
template auto plus_impl_el_matrix_el_matrix::operator()(el_matrix<El::Complex<float>>,  el_matrix<El::Complex<float>>  const&) const;
template auto plus_impl_el_matrix_el_matrix::operator()(el_matrix<double>,              el_matrix<double>              const&) const;
template auto plus_impl_el_matrix_el_matrix::operator()(el_matrix<El::Complex<double>>, el_matrix<El::Complex<double>> const&) const;

template auto plus_impl_el_matrix_scalar::operator()(el_matrix<float>,               float               const&) const;
template auto plus_impl_el_matrix_scalar::operator()(el_matrix<El::Complex<float>>,  El::Complex<float>  const&) const;
template auto plus_impl_el_matrix_scalar::operator()(el_matrix<double>,              double              const&) const;
template auto plus_impl_el_matrix_scalar::operator()(el_matrix<El::Complex<double>>, El::Complex<double> const&) const;

//TODO: Add dist_*_vector impls!

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
