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

#include <hbrs/mpl/fn/size.hpp>

#include <elemental/dt/vector.hpp>
#include <elemental/dt/dist_vector.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace detail {

template auto size_impl_Matrix::operator()(El::Matrix<float> const&) const;
template auto size_impl_Matrix::operator()(El::Matrix<El::Complex<float>> const&) const;
template auto size_impl_Matrix::operator()(El::Matrix<double> const&) const;
template auto size_impl_Matrix::operator()(El::Matrix<El::Complex<double>> const&) const;

template auto size_impl_AbstractDistMatrix::operator()(El::AbstractDistMatrix<float> const&) const;
template auto size_impl_AbstractDistMatrix::operator()(El::AbstractDistMatrix<El::Complex<float>> const&) const;
template auto size_impl_AbstractDistMatrix::operator()(El::AbstractDistMatrix<double> const&) const;
template auto size_impl_AbstractDistMatrix::operator()(El::AbstractDistMatrix<El::Complex<double>> const&) const;

template decltype(auto) length_impl_row_vector::operator()(row_vector<float> const&) const;
template decltype(auto) length_impl_row_vector::operator()(row_vector<El::Complex<float>> const&) const;
template decltype(auto) length_impl_row_vector::operator()(row_vector<double> const&) const;
template decltype(auto) length_impl_row_vector::operator()(row_vector<El::Complex<double>> const&) const;

template decltype(auto) length_impl_column_vector::operator()(column_vector<float> const&) const;
template decltype(auto) length_impl_column_vector::operator()(column_vector<El::Complex<float>> const&) const;
template decltype(auto) length_impl_column_vector::operator()(column_vector<double> const&) const;
template decltype(auto) length_impl_column_vector::operator()(column_vector<El::Complex<double>> const&) const;

// TODO: 
// template decltype(auto) length_impl_dist_row_vector::operator()(dist_row_vector<float> const&) const;
// template decltype(auto) length_impl_dist_row_vector::operator()(dist_row_vector<El::Complex<float>> const&) const;
// template decltype(auto) length_impl_dist_row_vector::operator()(dist_row_vector<double> const&) const;
// template decltype(auto) length_impl_dist_row_vector::operator()(dist_row_vector<El::Complex<double>> const&) const;
// 
// template decltype(auto) length_impl_dist_column_vector::operator()(dist_column_vector<float> const&) const;
// template decltype(auto) length_impl_dist_column_vector::operator()(dist_column_vector<El::Complex<float>> const&) const;
// template decltype(auto) length_impl_dist_column_vector::operator()(dist_column_vector<double> const&) const;
// template decltype(auto) length_impl_dist_column_vector::operator()(dist_column_vector<El::Complex<double>> const&) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END