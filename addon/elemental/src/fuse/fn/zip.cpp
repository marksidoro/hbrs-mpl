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

#include <hbrs/mpl/fn/zip.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace detail {

template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<float>               , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<El::Complex<float>>  , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<double>              , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<El::Complex<double>> , El::Int>, boost::integer_range<El::Int>) const;

template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<float>              const& , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<El::Complex<float>> const& , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<double>             const& , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smc_Matrix_integer_range::operator()(mpl::smc<El::Matrix<El::Complex<double>>const& , El::Int>, boost::integer_range<El::Int>) const;

template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<float>               , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<El::Complex<float>>  , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<double>              , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<El::Complex<double>> , El::Int>, boost::integer_range<El::Int>) const;

template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<float>              const& , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<El::Complex<float>> const& , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<double>             const& , El::Int>, boost::integer_range<El::Int>) const;
template auto zip_impl_smr_Matrix_integer_range::operator()(mpl::smr<El::Matrix<El::Complex<double>>const& , El::Int>, boost::integer_range<El::Int>) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END