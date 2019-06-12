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

#include <hbrs/mpl/fn/mean.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>

extern "C" {
	#include <matlab/cxn/mean_m.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

MATLAB_NAMESPACE_BEGIN
namespace detail {

mpl::smc<matlab::matrix<real_T>, int>
mean_impl_matrix::operator()(mpl::smrs<matlab::matrix<real_T>> const& a) const {
	matlab::matrix<real_T> b;
	mean_m(&a.data().data(), ::row_mean, &b.data());
	return {b, 0};
}

mpl::smr<matlab::matrix<real_T>, int>
mean_impl_matrix::operator()(mpl::smcs<matlab::matrix<real_T>> const& a) const {
	matlab::matrix<real_T> b;
	mean_m(&a.data().data(), ::column_mean, &b.data());
	return {b, 0};
}

/* namespace detail */ }
MATLAB_NAMESPACE_END