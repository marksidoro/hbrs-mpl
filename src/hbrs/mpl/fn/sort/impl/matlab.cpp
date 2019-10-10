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

#include "matlab.hpp"
#ifdef HBRS_MPL_ENABLE_MATLAB

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/ml_vector.hpp>
#include <hbrs/mpl/dt/eig_result.hpp>

#include <hbrs/mpl/detail/sort_eig_result.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

eig_result<ml_column_vector<real_T>, ml_matrix<real_T>>
sort_impl_ml_eig_result::operator()(
	eig_result<ml_column_vector<real_T>, ml_matrix<real_T>> const& r,
	std::function<bool(real_T, real_T)> const& p
) const {
	return sort_eig_result(r,p);
}

eig_result<ml_column_vector<creal_T>, ml_matrix<creal_T>>
sort_impl_ml_eig_result::operator()(
	eig_result<ml_column_vector<creal_T>, ml_matrix<creal_T>> const& r,
	std::function<bool(creal_T, creal_T)> const& p
) const {
	return sort_eig_result(r,p);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
