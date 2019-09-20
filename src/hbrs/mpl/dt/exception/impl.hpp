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

#ifndef HBRS_MPL_DT_EXCEPTION_IMPL_HPP
#define HBRS_MPL_DT_EXCEPTION_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <boost/exception/all.hpp>
#include <stdexcept>
#include <string>

HBRS_MPL_NAMESPACE_BEGIN

struct exception : virtual boost::exception, virtual std::exception {};
struct incompatible_sequence_exception : virtual exception {};
struct incompatible_sequences_exception : virtual exception {};
struct incompatible_vectors_exception : virtual exception {};
struct incompatible_matrix_exception : virtual exception {};
struct incompatible_matrix_sequence_exception : virtual exception {};
struct incompatible_matrix_vector_exception : virtual exception {};
struct incompatible_matrices_exception : virtual exception {};
struct mpi_exception : virtual exception {};
struct io_exception : virtual exception {};
struct not_supported_exception : virtual exception {};

struct mpi_error_info {
	mpi_error_info(int code);
	mpi_error_info(int code, int class_, std::string string);
	
	mpi_error_info(mpi_error_info const&) = default;
	mpi_error_info(mpi_error_info &&) = default;
	
	mpi_error_info&
	operator=(mpi_error_info const&) = default;
	mpi_error_info&
	operator=(mpi_error_info &&) = default;
	
	
	constexpr decltype(auto)
	code() & { return (code_); };
	
	constexpr decltype(auto)
	code() const& { return (code_); };
	
	constexpr decltype(auto)
	code() && { return HBRS_MPL_FWD(code_); };
	
	
	constexpr decltype(auto)
	class_() & { return (class__); };
	
	constexpr decltype(auto)
	class_() const& { return (class__); };
	
	constexpr decltype(auto)
	class_() && { return HBRS_MPL_FWD(class__); };
	
	
	constexpr decltype(auto)
	string() & { return (string_); };
	
	constexpr decltype(auto)
	string() const& { return (string_); };
	
	constexpr decltype(auto)
	string() && { return HBRS_MPL_FWD(string_); };
	
private:
	int code_;
	int class__;
	std::string string_;
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DT_EXCEPTION_IMPL_HPP
