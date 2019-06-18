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

#ifndef HBRS_MPL_FN_N_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_N_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

HBRS_MPL_DEC_FO_TRY_METHOD(n_impl_el_matrix, el_matrix_tag, n)
HBRS_MPL_DEC_FO_TRY_METHOD(n_impl_el_dist_matrix, el_dist_matrix_tag, n)

#else
struct n_impl_el_matrix{};
struct n_impl_el_dist_matrix{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_N_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                         \
		hbrs::mpl::detail::n_impl_el_matrix{},                                                                         \
		hbrs::mpl::detail::n_impl_el_dist_matrix{}                                                                     \
	)

#endif // !HBRS_MPL_FN_N_FWD_ELEMENTAL_HPP
