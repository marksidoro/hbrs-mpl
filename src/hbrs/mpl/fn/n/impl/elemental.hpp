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

#ifndef ELEMENTAL_FUSE_FN_N_HPP
#define ELEMENTAL_FUSE_FN_N_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

HBRS_MPL_DEF_FO_TRY_METHOD(n_impl_matrix, matrix_tag, n)
HBRS_MPL_DEF_FO_TRY_METHOD(n_impl_dist_matrix, dist_matrix_tag, n)

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_N_IMPLS boost::hana::make_tuple(                                                             \
		elemental::detail::n_impl_matrix{},                                                                            \
		elemental::detail::n_impl_dist_matrix{}                                                                        \
	)

#endif // !ELEMENTAL_FUSE_FN_N_HPP
