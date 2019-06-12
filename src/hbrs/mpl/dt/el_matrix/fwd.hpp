/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef ELEMENTAL_FWD_DT_MATRIX_HPP
#define ELEMENTAL_FWD_DT_MATRIX_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename Ring>
struct matrix;
struct matrix_tag;
constexpr auto make_matrix = hana::make<matrix_tag>;
constexpr auto to_matrix = hana::to<matrix_tag>;

ELEMENTAL_NAMESPACE_END

#endif // !ELEMENTAL_FWD_DT_MATRIX_HPP
