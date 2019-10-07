/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#include <hbrs/mpl/dt/ml_matrix.hpp>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/multiply_mdmd.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/multiply_mdmcd.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/multiply_mcdmcd.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/multiply_mdvd.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/multiply_mcdvcd.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

ml_matrix<real_T>
multiply_impl_ml_matrix_ml_matrix::operator()(ml_matrix<real_T> const& a, ml_matrix<real_T> const& b) const {
	ml_matrix<real_T> c;
	multiply_mdmd(&a.data(), &b.data(), &c.data());
	return c;
}

ml_matrix<creal_T>
multiply_impl_ml_matrix_ml_matrix::operator()(ml_matrix<real_T> const& a, ml_matrix<creal_T> const& b) const {
	ml_matrix<creal_T> c;
	multiply_mdmcd(&a.data(), &b.data(), &c.data());
	return c;
}

ml_matrix<creal_T>
multiply_impl_ml_matrix_ml_matrix::operator()(ml_matrix<creal_T> const& a, ml_matrix<creal_T> const& b) const {
	ml_matrix<creal_T> c;
	multiply_mcdmcd(&a.data(), &b.data(), &c.data());
	return c;
}

ml_column_vector<real_T>
multiply_impl_ml_matrix_ml_vector::operator()(ml_matrix<real_T> const& a, ml_column_vector<real_T> const& b) const {
	ml_column_vector<real_T> c;
	multiply_mdvd(&a.data(), &b.data(), &c.data());
	return c;
}

ml_column_vector<creal_T>
multiply_impl_ml_matrix_ml_vector::operator()(ml_matrix<creal_T> const& a, ml_column_vector<creal_T> const& b) const {
	ml_column_vector<creal_T> c;
	multiply_mcdvcd(&a.data(), &b.data(), &c.data());
	return c;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
