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

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/matlab_cxn_initialize.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/matlab_cxn_terminate.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

struct matlab_cxn {
	matlab_cxn() {
		matlab_cxn_initialize();
	}
	
	~matlab_cxn() {
		matlab_cxn_terminate();
	}
};

matlab_cxn cxn{};
