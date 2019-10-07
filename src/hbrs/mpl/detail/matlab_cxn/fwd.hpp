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

#ifndef HBRS_MPL_DETAIL_MATLAB_CXN_FWD_HPP
#define HBRS_MPL_DETAIL_MATLAB_CXN_FWD_HPP

#include <hbrs/mpl/config.hpp>
#ifdef HBRS_MPL_ENABLE_MATLAB

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/matlab_cxn_emxAPI.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

#include <ostream>
#include <complex>

std::complex<double>
to_std_complex(creal_T const& x);

creal_T
from_std_complex(std::complex<real_T> const& x);

std::ostream&
operator<<(std::ostream& o, creal_T const& x);

bool
operator==(creal_T const& lhs, double const& rhs);

bool
operator==(creal_T const& lhs, std::complex<double> const& rhs);

bool
operator==(creal_T const& lhs, creal_T const& rhs);

#endif // !HBRS_MPL_ENABLE_MATLAB

#endif // !HBRS_MPL_DETAIL_MATLAB_CXN_FWD_HPP