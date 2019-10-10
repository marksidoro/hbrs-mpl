/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#include <functional>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_column_vector<float>,
		el_matrix<float>
	> const&,
	std::function<bool(float, float)> const&
) const;

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_column_vector<El::Complex<float>>,
		el_matrix<El::Complex<float>>
	> const&,
	std::function<bool(El::Complex<float>, El::Complex<float>)> const&
) const;

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_column_vector<double>,
		el_matrix<double>
	> const&,
	std::function<bool(double, double)> const&
) const;

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_column_vector<El::Complex<double>>,
		el_matrix<El::Complex<double>>
	> const&,
	std::function<bool(El::Complex<double>, El::Complex<double>)> const&
) const;

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_dist_column_vector<float>,
		el_dist_matrix<float>
	> const&,
	std::function<bool(float, float)> const&
) const;

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_dist_column_vector<El::Complex<float>>,
		el_dist_matrix<El::Complex<float>>
	> const&,
	std::function<bool(El::Complex<float>, El::Complex<float>)> const&
) const;

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_dist_column_vector<double>,
		el_dist_matrix<double>
	> const&,
	std::function<bool(double, double)> const&
) const;

template
auto
sort_impl_el_eig_result::operator()(
	eig_result<
		el_dist_column_vector<El::Complex<double>>,
		el_dist_matrix<El::Complex<double>>
	> const&,
	std::function<bool(El::Complex<double>, El::Complex<double>)> const&
) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
