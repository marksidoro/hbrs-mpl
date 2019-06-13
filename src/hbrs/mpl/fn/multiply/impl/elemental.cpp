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

#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/dt/scv.hpp>
#include <vector>

HBRS_MPL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

template decltype(auto) multiply_impl_matrix_matrix::operator()(matrix<float>               const&, matrix<float>               const&) const;
template decltype(auto) multiply_impl_matrix_matrix::operator()(matrix<El::Complex<float>>  const&, matrix<El::Complex<float>>  const&) const;
template decltype(auto) multiply_impl_matrix_matrix::operator()(matrix<double>              const&, matrix<double>              const&) const;
template decltype(auto) multiply_impl_matrix_matrix::operator()(matrix<El::Complex<double>> const&, matrix<El::Complex<double>> const&) const;

template decltype(auto) multiply_impl_dist_matrix_dist_matrix::operator()(dist_matrix<float>               const&, dist_matrix<float>               const&) const;
template decltype(auto) multiply_impl_dist_matrix_dist_matrix::operator()(dist_matrix<El::Complex<float>>  const&, dist_matrix<El::Complex<float>>  const&) const;
template decltype(auto) multiply_impl_dist_matrix_dist_matrix::operator()(dist_matrix<double>              const&, dist_matrix<double>              const&) const;
template decltype(auto) multiply_impl_dist_matrix_dist_matrix::operator()(dist_matrix<El::Complex<double>> const&, dist_matrix<El::Complex<double>> const&) const;

template auto multiply_impl_matrix_scv_vector::operator()(matrix<float>              const&, mpl::scv<std::vector<int>> const&) const;
//TODO: Solve "error: call to 'operator*' is ambiguous" for El::Complex<...> types
// template auto multiply_impl_matrix_scv_vector::operator()(matrix<El::Complex<float>> const&, mpl::scv<std::vector<int>> const&) const;
template auto multiply_impl_matrix_scv_vector::operator()(matrix<double>             const&, mpl::scv<std::vector<int>> const&) const;
// template auto multiply_impl_matrix_scv_vector::operator()(matrix<El::Complex<double>>const&, mpl::scv<std::vector<int>> const&) const;

template auto multiply_impl_matrix_scalar::operator()(matrix<float>              , float               const&) const;
template auto multiply_impl_matrix_scalar::operator()(matrix<El::Complex<float>> , El::Complex<float>  const&) const;
template auto multiply_impl_matrix_scalar::operator()(matrix<double>             , double              const&) const;
template auto multiply_impl_matrix_scalar::operator()(matrix<El::Complex<double>>, El::Complex<double> const&) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END