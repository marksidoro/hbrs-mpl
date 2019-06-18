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

#include "elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

//TODO: Add transform_impl_vector and transform_impl_dist_vector impls!

template auto transform_impl_el_matrix::operator()(el_matrix<float>              const&, std::function<              float(float              )> const&) const;
template auto transform_impl_el_matrix::operator()(el_matrix<El::Complex<float>> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_el_matrix::operator()(el_matrix<double>             const&, std::function<             double(double             )> const&) const;
template auto transform_impl_el_matrix::operator()(el_matrix<El::Complex<double>>const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template auto transform_impl_smr_el_matrix::operator()(smr<el_matrix<float>              , El::Int> const&, std::function<              float(float              )> const&) const;
template auto transform_impl_smr_el_matrix::operator()(smr<el_matrix<El::Complex<float>> , El::Int> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_smr_el_matrix::operator()(smr<el_matrix<double>             , El::Int> const&, std::function<             double(double             )> const&) const;
template auto transform_impl_smr_el_matrix::operator()(smr<el_matrix<El::Complex<double>>, El::Int> const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template auto transform_impl_smc_el_matrix::operator()(smc<el_matrix<float>              , El::Int> const&, std::function<              float(float              )> const&) const;
template auto transform_impl_smc_el_matrix::operator()(smc<el_matrix<El::Complex<float>> , El::Int> const&, std::function< El::Complex<float>(El::Complex<float> )> const&) const;
template auto transform_impl_smc_el_matrix::operator()(smc<el_matrix<double>             , El::Int> const&, std::function<             double(double             )> const&) const;
template auto transform_impl_smc_el_matrix::operator()(smc<el_matrix<El::Complex<double>>, El::Int> const&, std::function<El::Complex<double>(El::Complex<double>)> const&) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smcs<el_matrix<float>> const&, 
	std::function<
		float(
			smc<el_matrix<float> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smcs<el_matrix<El::Complex<float>>> const&, 
	std::function<
		float(
			smc<el_matrix<El::Complex<float>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smcs<el_matrix<double>> const&, 
	std::function<
		double(
			smc<el_matrix<double> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smcs<el_matrix<El::Complex<double>>> const&, 
	std::function<
		double(
			smc<el_matrix<El::Complex<double>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smrs<el_matrix<float>> const&, 
	std::function<
		float(
			smr<el_matrix<float> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smrs<el_matrix<El::Complex<float>>> const&, 
	std::function<
		float(
			smr<el_matrix<El::Complex<float>> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smrs<el_matrix<double>> const&, 
	std::function<
		double(
			smr<el_matrix<double> const&, El::Int> const&
		)
	> const&
) const;

template decltype(auto) transform_impl_smcs_smrs_el_matrix::operator()(
	smrs<el_matrix<El::Complex<double>>> const&, 
	std::function<
		double(
			smr<el_matrix<El::Complex<double>> const&, El::Int> const&
		)
	> const&
) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
