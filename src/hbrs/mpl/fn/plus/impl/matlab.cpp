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

#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/plus_mm.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/plus_ms.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

hbrs::mpl::ml_matrix<real_T>
plus_impl_matrix_scalar::operator()(hbrs::mpl::ml_matrix<real_T> const& a, real_T b) const {
	hbrs::mpl::ml_matrix<real_T> c;
	plus_ms(&a.data(), b, &c.data());
	return c;
}

hbrs::mpl::ml_matrix<real_T>
plus_impl_matrix_matrix::operator()(hbrs::mpl::ml_matrix<real_T> const& a, hbrs::mpl::ml_matrix<real_T> const& b) const {
	hbrs::mpl::ml_matrix<real_T> c;
	plus_mm(&a.data(), &b.data(), &c.data());
	return c;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END