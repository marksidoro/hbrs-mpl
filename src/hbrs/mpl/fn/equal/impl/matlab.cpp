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

#include <hbrs/mpl/detail/matlab_cxn.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

bool
equal_impl_ml_creal_T::operator()(real_T const& lhs, creal_T const& rhs) const {
	return (rhs.re == lhs) && (rhs.im == 0.);
}

bool
equal_impl_ml_creal_T::operator()(creal_T const& lhs, real_T const& rhs) const {
	return (lhs.re == rhs) && (lhs.im == 0.);
}

bool
equal_impl_ml_creal_T::operator()(creal_T const& lhs, creal_T const& rhs) const {
	return (lhs.re == rhs.re) && (lhs.im == rhs.im);
}

bool
equal_impl_ml_creal_T::operator()(creal_T const& lhs, std::complex<real_T> const& rhs) const {
	return (lhs.re == rhs.real()) && (lhs.im == rhs.imag());
}

bool
equal_impl_ml_creal_T::operator()(std::complex<real_T> const& lhs, creal_T const& rhs) const {
	return (lhs.real() == rhs.re) && (lhs.imag() == rhs.im);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
