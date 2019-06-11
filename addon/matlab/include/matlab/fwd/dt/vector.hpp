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

#ifndef MATLAB_FWD_DT_VECTOR_HPP
#define MATLAB_FWD_DT_VECTOR_HPP

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
struct column_vector;
struct column_vector_tag;
constexpr auto make_column_vector = hana::make<column_vector_tag>;
constexpr auto to_column_vector = hana::to<column_vector_tag>;

template<typename BaseType>
struct row_vector;
struct row_vector_tag;
constexpr auto make_row_vector = hana::make<row_vector_tag>;
constexpr auto to_row_vector = hana::to<row_vector_tag>;

MATLAB_NAMESPACE_END



#endif // !MATLAB_FWD_DT_VECTOR_HPP
