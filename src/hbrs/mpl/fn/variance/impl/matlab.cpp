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
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smrs.hpp>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/variance_md.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

smc<ml_matrix<real_T>, int>
variance_impl_ml_matrix::operator()(smrs<ml_matrix<real_T>> const& a, real_T weight) const {
	ml_matrix<real_T> b;
	variance_md(&a.data().data(), weight, ::row, &b.data());
	return {b, 0};
}

smr<ml_matrix<real_T>, int>
variance_impl_ml_matrix::operator()(smcs<ml_matrix<real_T>> const& a, real_T weight) const {
	ml_matrix<real_T> b;
	variance_md(&a.data().data(), weight, ::column, &b.data());
	return {b, 0};
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
