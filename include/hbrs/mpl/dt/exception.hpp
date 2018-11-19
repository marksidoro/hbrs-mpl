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

#pragma once

#ifndef HBRS_MPL_DT_EXCEPTION_HPP
#define HBRS_MPL_DT_EXCEPTION_HPP

#include <hbrs/mpl/fwd/dt/exception.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <boost/exception/all.hpp>
#include <stdexcept>

HBRS_MPL_NAMESPACE_BEGIN

struct exception : virtual boost::exception, virtual std::exception {};
struct incompatible_sequence_exception : virtual mpl::exception {};
struct incompatible_sequences_exception : virtual mpl::exception {};
struct incompatible_vectors_exception : virtual mpl::exception {};
struct incompatible_matrix_exception : virtual mpl::exception {};
struct incompatible_matrix_sequence_exception : virtual mpl::exception {};
struct incompatible_matrix_vector_exception : virtual mpl::exception {};
struct incompatible_matrices_exception : virtual mpl::exception {};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DT_EXCEPTION_HPP
