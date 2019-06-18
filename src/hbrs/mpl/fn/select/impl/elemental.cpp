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

//TODO: Why does compilation fail for those specalizations???
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<float>               const&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<El::Complex<float>>  const&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<double>              const&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<El::Complex<double>> const&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// 
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<float>               &&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<El::Complex<float>>  &&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<double>              &&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<El::Complex<double>> &&,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// 
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<float>               &,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<El::Complex<float>>  &,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<double>              &,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_el_matrix::operator()(el_matrix<El::Complex<double>> &,
// 	range<matrix_index<El::Int, El::Int>, matrix_index<El::Int, El::Int>> const&) const;

template el_matrix<float               const> select_impl_el_matrix::operator()(el_matrix<float>               const&, std::pair<El::IR, El::IR> const&) const;
template el_matrix<El::Complex<float>  const> select_impl_el_matrix::operator()(el_matrix<El::Complex<float>>  const&, std::pair<El::IR, El::IR> const&) const;
template el_matrix<double              const> select_impl_el_matrix::operator()(el_matrix<double>              const&, std::pair<El::IR, El::IR> const&) const;
template el_matrix<El::Complex<double> const> select_impl_el_matrix::operator()(el_matrix<El::Complex<double>> const&, std::pair<El::IR, El::IR> const&) const;

// template el_matrix<Ring> select_impl_el_matrix::operator()(el_matrix<float>               &&, std::pair<El::IR, El::IR> const&) const;
// template el_matrix<Ring> select_impl_el_matrix::operator()(el_matrix<El::Complex<float>>  &&, std::pair<El::IR, El::IR> const&) const;
// template el_matrix<Ring> select_impl_el_matrix::operator()(el_matrix<double>              &&, std::pair<El::IR, El::IR> const&) const;
// template el_matrix<Ring> select_impl_el_matrix::operator()(el_matrix<El::Complex<double>> &&, std::pair<El::IR, El::IR> const&) const;

template el_matrix<float              > select_impl_el_matrix::operator()(el_matrix<float>               &, std::pair<El::IR, El::IR> const&) const;
template el_matrix<El::Complex<float> > select_impl_el_matrix::operator()(el_matrix<El::Complex<float>>  &, std::pair<El::IR, El::IR> const&) const;
template el_matrix<double             > select_impl_el_matrix::operator()(el_matrix<double>              &, std::pair<El::IR, El::IR> const&) const;
template el_matrix<El::Complex<double>> select_impl_el_matrix::operator()(el_matrix<El::Complex<double>> &, std::pair<El::IR, El::IR> const&) const;

//TODO: More!!
//TODO: Add Dist* impls!

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
