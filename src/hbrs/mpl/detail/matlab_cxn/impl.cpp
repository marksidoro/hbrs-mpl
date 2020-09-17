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

#include "impl.hpp"

#ifdef HBRS_MPL_ENABLE_MATLAB

HBRS_MPL_API
std::complex<double>
to_std_complex(creal_T const& x) {
	return {x.re, x.im};
}

HBRS_MPL_API
creal_T
from_std_complex(std::complex<real_T> const& x) {
	return {x.real(), x.imag()};
}

HBRS_MPL_API
std::ostream&
operator<<(std::ostream& o, creal_T const& x) {
	return o << to_std_complex(x);
}

HBRS_MPL_API
bool
operator==(creal_T const& lhs, double const& rhs) {
	return to_std_complex(lhs) == rhs;
}

HBRS_MPL_API
bool
operator==(creal_T const& lhs, creal_T const& rhs) {
	return to_std_complex(lhs) == to_std_complex(rhs);
}

HBRS_MPL_API
bool
operator==( creal_T const& lhs, std::complex<double> const& rhs) {
	return to_std_complex(lhs) == rhs;
}

#endif // !HBRS_MPL_ENABLE_MATLAB
