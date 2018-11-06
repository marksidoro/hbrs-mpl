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

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace detail {

template decltype(auto) at_impl_Matrix::operator()(El::Matrix<float> &,      mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<float> const&, mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<float> &&,    mpl::matrix_index<El::Int, El::Int> const&) const;

template decltype(auto) at_impl_Matrix::operator()(El::Matrix<El::Complex<float>> &,      mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<El::Complex<float>> const&, mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<El::Complex<float>> &&,    mpl::matrix_index<El::Int, El::Int> const&) const;

template decltype(auto) at_impl_Matrix::operator()(El::Matrix<double> &,      mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<double> const&, mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<double> &&,    mpl::matrix_index<El::Int, El::Int> const&) const;

template decltype(auto) at_impl_Matrix::operator()(El::Matrix<El::Complex<double>> &,      mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<El::Complex<double>> const&, mpl::matrix_index<El::Int, El::Int> const&) const;
template decltype(auto) at_impl_Matrix::operator()(El::Matrix<El::Complex<double>> &&,    mpl::matrix_index<El::Int, El::Int> const&) const;

template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<float> &,      El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<float> const&, El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<float> &&,    El::Int) const;

template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<El::Complex<float>> &,      El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<El::Complex<float>> const&, El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<El::Complex<float>> &&,    El::Int) const;

template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<double> &,      El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<double> const&, El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<double> &&,    El::Int) const;

template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<El::Complex<double>> &,      El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<El::Complex<double>> const&, El::Int) const;
template decltype(auto) at_impl_Matrix_smr::operator()(El::Matrix<El::Complex<double>> &&,    El::Int) const;

//TODO: Add row_vector and column_vector impls!

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END