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

#ifndef HBRS_MPL_DT_EXCEPTION_FWD_HPP
#define HBRS_MPL_DT_EXCEPTION_FWD_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
#include <boost/exception/error_info.hpp>
#include <boost/system/error_code.hpp>
#include <tuple>

HBRS_MPL_NAMESPACE_BEGIN

struct exception;
struct incompatible_sequence_exception;
struct incompatible_sequences_exception;
struct incompatible_vectors_exception;
struct incompatible_matrix_exception;
struct incompatible_matrix_sequence_exception;
struct incompatible_matrix_vector_exception;
struct incompatible_matrices_exception;
struct mpi_exception;
struct io_exception;

typedef boost::error_info<struct errinfo_sequence_size_, std::size_t > errinfo_sequence_size;
typedef boost::error_info<
	struct errinfo_sequences_sizes_, 
	std::tuple< std::size_t, std::size_t>
> errinfo_sequences_sizes;

typedef boost::error_info<struct errinfo_vector_sizes_, std::tuple<std::size_t, std::size_t> >
	errinfo_vector_sizes;
typedef boost::error_info<struct errinfo_vector_size_, std::size_t > 
	errinfo_vector_size;

typedef boost::error_info<struct errinfo_matrix_size_, matrix_size<std::size_t, std::size_t> >
	errinfo_matrix_size;

typedef boost::error_info<
	struct errinfo_matrix_sizes_,
	std::tuple< 
		matrix_size<std::size_t, std::size_t>,
		matrix_size<std::size_t, std::size_t>
	>
> errinfo_matrix_sizes;

struct mpi_error_info;
typedef boost::error_info<struct errinfo_mpi_error_info_, mpi_error_info> errinfo_mpi_error_info;

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DT_EXCEPTION_FWD_HPP
