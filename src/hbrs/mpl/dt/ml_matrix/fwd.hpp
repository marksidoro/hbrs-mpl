/* Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef MATLAB_FWD_DT_MATRIX_HPP
#define MATLAB_FWD_DT_MATRIX_HPP

#include <matlab/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

extern "C" {
	#include <matlab/cxn/matlab_cxn_emxAPI.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename BaseType>
struct matrix;
struct matrix_tag {};
constexpr auto make_matrix = hana::make<matrix_tag>;
constexpr auto to_matrix = hana::to<matrix_tag>;

MATLAB_NAMESPACE_END

#endif // !MATLAB_FWD_DT_MATRIX_HPP
