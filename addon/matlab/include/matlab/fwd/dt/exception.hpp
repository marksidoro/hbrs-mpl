/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef MATLAB_FWD_DT_EXCEPTION_HPP
#define MATLAB_FWD_DT_EXCEPTION_HPP

#include <matlab/config.hpp>
#include <hbrs/mpl/fwd/dt/exception.hpp>
#include <hbrs/mpl/fwd/dt/matrix_size.hpp>
#include <boost/exception/error_info.hpp>
#include <tuple>

MATLAB_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;

typedef boost::error_info<struct errinfo_vector_sizes_, std::tuple<int, int>  >
	errinfo_vector_sizes;
typedef boost::error_info<struct errinfo_vector_size_, int >
	errinfo_vector_size;
typedef boost::error_info<struct errinfo_matrix_size_, mpl::matrix_size<int, int> >
	errinfo_matrix_size;

MATLAB_NAMESPACE_END

#endif // !MATLAB_FWD_DT_EXCEPTION_HPP
