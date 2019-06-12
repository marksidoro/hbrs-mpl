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

//TODO: Add transform_impl_vector and transform_impl_dist_vector impls!

template auto transform_impl_matrix::operator()(matrix<float>              const&, std::function<              float(float              )> const&) const;
template auto transform_impl_matrix::operator()(matrix<El::Complex<float>> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_matrix::operator()(matrix<double>             const&, std::function<             double(double             )> const&) const;
template auto transform_impl_matrix::operator()(matrix<El::Complex<double>>const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template auto transform_impl_smr_matrix::operator()(mpl::smr<matrix<float>              , El::Int> const&, std::function<              float(float              )> const&) const;
template auto transform_impl_smr_matrix::operator()(mpl::smr<matrix<El::Complex<float>> , El::Int> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_smr_matrix::operator()(mpl::smr<matrix<double>             , El::Int> const&, std::function<             double(double             )> const&) const;
template auto transform_impl_smr_matrix::operator()(mpl::smr<matrix<El::Complex<double>>, El::Int> const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template auto transform_impl_smc_matrix::operator()(mpl::smc<matrix<float>              , El::Int> const&, std::function<              float(float              )> const&) const;
template auto transform_impl_smc_matrix::operator()(mpl::smc<matrix<El::Complex<float>> , El::Int> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_smc_matrix::operator()(mpl::smc<matrix<double>             , El::Int> const&, std::function<             double(double             )> const&) const;
template auto transform_impl_smc_matrix::operator()(mpl::smc<matrix<El::Complex<double>>, El::Int> const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smcs<matrix<float>> const&, 
	std::function<
		float(
			mpl::smc<matrix<float> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smcs<matrix<El::Complex<float>>> const&, 
	std::function<
		float(
			mpl::smc<matrix<El::Complex<float>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smcs<matrix<double>> const&, 
	std::function<
		double(
			mpl::smc<matrix<double> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smcs<matrix<El::Complex<double>>> const&, 
	std::function<
		double(
			mpl::smc<matrix<El::Complex<double>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smrs<matrix<float>> const&, 
	std::function<
		float(
			mpl::smr<matrix<float> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smrs<matrix<El::Complex<float>>> const&, 
	std::function<
		float(
			mpl::smr<matrix<El::Complex<float>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smrs<matrix<double>> const&, 
	std::function<
		double(
			mpl::smr<matrix<double> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_matrix::operator()(
	mpl::smrs<matrix<El::Complex<double>>> const&, 
	std::function<
		double(
			mpl::smr<matrix<El::Complex<double>> const&, El::Int> const&
		)
	> const&
) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END