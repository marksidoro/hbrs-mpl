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

#include <hbrs/mpl/fn/transform.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace detail {

template auto transform_impl_Matrix::operator()(El::Matrix<float>              , std::function<              float(float              )> const&) const;
template auto transform_impl_Matrix::operator()(El::Matrix<El::Complex<float>> , std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_Matrix::operator()(El::Matrix<double>             , std::function<             double(double             )> const&) const;
template auto transform_impl_Matrix::operator()(El::Matrix<El::Complex<double>>, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template auto transform_impl_smr_Matrix::operator()(mpl::smr<El::Matrix<float>              , El::Int> const&, std::function<              float(float              )> const&) const;
template auto transform_impl_smr_Matrix::operator()(mpl::smr<El::Matrix<El::Complex<float>> , El::Int> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_smr_Matrix::operator()(mpl::smr<El::Matrix<double>             , El::Int> const&, std::function<             double(double             )> const&) const;
template auto transform_impl_smr_Matrix::operator()(mpl::smr<El::Matrix<El::Complex<double>>, El::Int> const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template auto transform_impl_smc_Matrix::operator()(mpl::smc<El::Matrix<float>              , El::Int> const&, std::function<              float(float              )> const&) const;
template auto transform_impl_smc_Matrix::operator()(mpl::smc<El::Matrix<El::Complex<float>> , El::Int> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_smc_Matrix::operator()(mpl::smc<El::Matrix<double>             , El::Int> const&, std::function<             double(double             )> const&) const;
template auto transform_impl_smc_Matrix::operator()(mpl::smc<El::Matrix<El::Complex<double>>, El::Int> const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smcs<El::Matrix<float>> const&, 
	std::function<
		float(
			mpl::smc<El::Matrix<float> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smcs<El::Matrix<El::Complex<float>>> const&, 
	std::function<
		float(
			mpl::smc<El::Matrix<El::Complex<float>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smcs<El::Matrix<double>> const&, 
	std::function<
		double(
			mpl::smc<El::Matrix<double> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smcs<El::Matrix<El::Complex<double>>> const&, 
	std::function<
		double(
			mpl::smc<El::Matrix<El::Complex<double>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smrs<El::Matrix<float>> const&, 
	std::function<
		float(
			mpl::smr<El::Matrix<float> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smrs<El::Matrix<El::Complex<float>>> const&, 
	std::function<
		float(
			mpl::smr<El::Matrix<El::Complex<float>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smrs<El::Matrix<double>> const&, 
	std::function<
		double(
			mpl::smr<El::Matrix<double> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_Matrix::operator()(
	mpl::smrs<El::Matrix<El::Complex<double>>> const&, 
	std::function<
		double(
			mpl::smr<El::Matrix<El::Complex<double>> const&, El::Int> const&
		)
	> const&
) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END