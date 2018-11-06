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

#include <hbrs/mpl/fn/mean.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;

namespace detail {

template auto mean_impl_smcs_Matrix::operator()(mpl::smcs<El::Matrix<float>> const&) const;
template auto mean_impl_smcs_Matrix::operator()(mpl::smcs<El::Matrix<El::Complex<float>>> const&) const;
template auto mean_impl_smcs_Matrix::operator()(mpl::smcs<El::Matrix<double>> const&) const;
template auto mean_impl_smcs_Matrix::operator()(mpl::smcs<El::Matrix<El::Complex<double>>> const&) const;

template auto mean_impl_smrs_Matrix::operator()(mpl::smrs<El::Matrix<float>> const&) const;
template auto mean_impl_smrs_Matrix::operator()(mpl::smrs<El::Matrix<El::Complex<float>>> const&) const;
template auto mean_impl_smrs_Matrix::operator()(mpl::smrs<El::Matrix<double>> const&) const;
template auto mean_impl_smrs_Matrix::operator()(mpl::smrs<El::Matrix<El::Complex<double>>> const&) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END