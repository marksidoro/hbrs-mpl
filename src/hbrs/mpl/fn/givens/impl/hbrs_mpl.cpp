/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
 * Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
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

#include "hbrs_mpl.hpp"

#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

/*
 * Algorithm 5.1.3 (Calculate Givens scalars) on page 240
 * --     --T   --   --         --   --
 * |       |    |     |         |     |
 * |  c s  |    |  a  |         |  r  |
 * |       |    |     |    =    |     |
 * | -s c  |    |  b  |         |  0  |
 * |       |    |     |         |     |
 * --     --    --   --         --   --
 *
 */
givens_result<double, double>
givens_impl::operator()(double a, double b) const {
	givens_result<double, double> cs;
	if (b == 0) {
		cs.c() = 1;
		cs.s() = 0;
	} else {
		if (std::abs(b) > std::abs(a)) {
			auto tau = -a / b;
			cs.s() = 1 / std::sqrt(1 + tau * tau);
			cs.c() = cs.s() * tau;
		} else {
			auto tau = -b / a;
			cs.c() = 1 / std::sqrt(1 + tau * tau);
			cs.s() = cs.c() * tau;
		}
	}
	return cs;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

