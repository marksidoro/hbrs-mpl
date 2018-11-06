
/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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

#pragma once

#ifndef ELEMENTAL_FWD_DT_EXCEPTION_HPP
#define ELEMENTAL_FWD_DT_EXCEPTION_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/fwd/dt/exception.hpp>
#include <hbrs/mpl/fwd/dt/matrix_size.hpp>
#include <boost/exception/error_info.hpp>
#include <tuple>
#include <El.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;

typedef boost::error_info<struct errinfo_sequence_size_, El::Int >
	errinfo_sequence_size;
typedef boost::error_info<struct errinfo_vector_sizes_, std::tuple<El::Int, El::Int>  > 
	errinfo_vector_sizes;
typedef boost::error_info<struct errinfo_vector_size_, El::Int > 
	errinfo_vector_size;
typedef boost::error_info<struct errinfo_matrix_size_, mpl::matrix_size<El::Int, El::Int> > 
	errinfo_matrix_size;
typedef boost::error_info<
	struct errinfo_matrix_sizes_,
	std::tuple<
		mpl::matrix_size<El::Int, El::Int>,
		mpl::matrix_size<El::Int, El::Int>
	>
> errinfo_matrix_sizes;

ELEMENTAL_NAMESPACE_END

#endif // !ELEMENTAL_FWD_DT_EXCEPTION_HPP
