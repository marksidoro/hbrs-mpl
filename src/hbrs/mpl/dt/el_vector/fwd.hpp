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

#ifndef HBRS_MPL_DT_EL_VECTOR_FWD_IMPL_HPP
#define HBRS_MPL_DT_EL_VECTOR_FWD_IMPL_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename Ring>
struct el_column_vector;
struct el_column_vector_tag;
constexpr auto make_el_column_vector = hana::make<el_column_vector_tag>;
constexpr auto to_el_column_vector = hana::to<el_column_vector_tag>;

template<typename Ring>
struct el_row_vector;
struct el_row_vector_tag;
constexpr auto make_el_row_vector = hana::make<el_row_vector_tag>;
constexpr auto to_el_row_vector = hana::to<el_row_vector_tag>;

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DT_EL_VECTOR_FWD_IMPL_HPP
