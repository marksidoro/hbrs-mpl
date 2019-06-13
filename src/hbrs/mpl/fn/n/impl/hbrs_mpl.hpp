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

#ifndef HBRS_MPL_FUSE_HBRS_MPL_FN_N_HPP
#define HBRS_MPL_FUSE_HBRS_MPL_FN_N_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/detail/function_object.hpp>
#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
#include <hbrs/mpl/dt/matrix_index/fwd.hpp>
#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

HBRS_MPL_DEF_FO_TRY_METHOD(n_impl_matrix_size,        matrix_size_tag,        n)
HBRS_MPL_DEF_FO_TRY_METHOD(n_impl_matrix_index,       matrix_index_tag,       n)

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_N_IMPLS_HBRS_MPL boost::hana::make_tuple(                                                     \
		hbrs::mpl::detail::n_impl_matrix_size{},                                                                       \
		hbrs::mpl::detail::n_impl_matrix_index{}                                                                       \
	)

#endif // !HBRS_MPL_FUSE_HBRS_MPL_FN_N_HPP
