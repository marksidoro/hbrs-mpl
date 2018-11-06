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

#include <hbrs/mpl/fn/vertcat.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace detail {

template auto vertcat_impl_smr_smr::operator()(
	mpl::smr<El::Matrix<float>, El::Int> const&,
	mpl::smr<El::Matrix<float>, El::Int> const&
) const;

template auto vertcat_impl_smr_smr::operator()(
	mpl::smr<El::Matrix<El::Complex<float>>, El::Int> const&,
	mpl::smr<El::Matrix<El::Complex<float>>, El::Int> const&
) const;

template auto vertcat_impl_smr_smr::operator()(
	mpl::smr<El::Matrix<double>, El::Int> const&,
	mpl::smr<El::Matrix<double>, El::Int> const&
) const;

template auto vertcat_impl_smr_smr::operator()(
	mpl::smr<El::Matrix<El::Complex<double>>, El::Int> const&,
	mpl::smr<El::Matrix<El::Complex<double>>, El::Int> const&
) const;

template auto vertcat_impl_Matrix_smr::operator()(
	El::Matrix<float> const&,
	mpl::smr<El::Matrix<float>, El::Int> const&
) const;

template auto vertcat_impl_Matrix_smr::operator()(
	El::Matrix<El::Complex<float>> const&,
	mpl::smr<El::Matrix<El::Complex<float>>, El::Int> const&
) const;

template auto vertcat_impl_Matrix_smr::operator()(
	El::Matrix<double> const&,
	mpl::smr<El::Matrix<double>, El::Int> const&
) const;

template auto vertcat_impl_Matrix_smr::operator()(
	El::Matrix<El::Complex<double>> const&,
	mpl::smr<El::Matrix<El::Complex<double>>, El::Int> const&
) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END