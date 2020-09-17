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
#include <hbrs/mpl/dt/ml_vector.hpp>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/complex_vdd.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/complex_vdvd.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

HBRS_MPL_API
ml_column_vector<creal_T>
complex_impl_ml_vector::operator()(ml_column_vector<real_T> const& real, real_T const& imag) const {
	ml_column_vector<creal_T> v;
	complex_vdd(&real.data(), imag, &v.data());
	return v;
}

HBRS_MPL_API
ml_column_vector<creal_T>
complex_impl_ml_vector::operator()(ml_column_vector<real_T> const& real, ml_column_vector<real_T> const& imag) const {
	ml_column_vector<creal_T> v;
	complex_vdvd(&real.data(), &imag.data(), &v.data());
	return v;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
