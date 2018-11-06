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

#include <hbrs/mpl/fn/select.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

//TODO: Why does compilation fail for those specalizations???
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<float>               const&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<El::Complex<float>>  const&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<double>              const&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<El::Complex<double>> const&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// 
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<float>               &&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<El::Complex<float>>  &&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<double>              &&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<El::Complex<double>> &&,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// 
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<float>               &,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<El::Complex<float>>  &,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<double>              &,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;
// template decltype(auto) select_impl_Matrix::operator()(El::Matrix<El::Complex<double>> &,
// 	mpl::range<mpl::matrix_index<El::Int, El::Int>, mpl::matrix_index<El::Int, El::Int>> const&) const;

template auto select_impl_Matrix::operator()(El::Matrix<float>               const&, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<El::Complex<float>>  const&, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<double>              const&, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<El::Complex<double>> const&, std::pair<El::IR, El::IR> const&) const;

template auto select_impl_Matrix::operator()(El::Matrix<float>               &&, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<El::Complex<float>>  &&, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<double>              &&, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<El::Complex<double>> &&, std::pair<El::IR, El::IR> const&) const;

template auto select_impl_Matrix::operator()(El::Matrix<float>               &, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<El::Complex<float>>  &, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<double>              &, std::pair<El::IR, El::IR> const&) const;
template auto select_impl_Matrix::operator()(El::Matrix<El::Complex<double>> &, std::pair<El::IR, El::IR> const&) const;

//TODO: More!!

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END
