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

#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template decltype(auto) size_impl_matrix::operator()(matrix<float> const&) const;
template decltype(auto) size_impl_matrix::operator()(matrix<El::Complex<float>> const&) const;
template decltype(auto) size_impl_matrix::operator()(matrix<double> const&) const;
template decltype(auto) size_impl_matrix::operator()(matrix<El::Complex<double>> const&) const;

template decltype(auto) size_impl_dist_matrix::operator()(dist_matrix<float> const&) const;
template decltype(auto) size_impl_dist_matrix::operator()(dist_matrix<El::Complex<float>> const&) const;
template decltype(auto) size_impl_dist_matrix::operator()(dist_matrix<double> const&) const;
template decltype(auto) size_impl_dist_matrix::operator()(dist_matrix<El::Complex<double>> const&) const;

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
HBRS_MPL_NAMESPACE_END