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

#ifndef HBRS_MPL_FN_COLUMNS_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_COLUMNS_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>
#include <vector>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
struct columns_impl_el_matrix_1 {
	template <
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		>* = nullptr
	>
	constexpr auto
	operator()(Matrix && a) const;
};

struct columns_impl_el_matrix_2 {
	template <typename Ring>
	constexpr auto
	operator()(el_matrix<Ring> const& a) const;
};

struct columns_impl_el_dist_matrix {
	template <
		typename DistMatrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value
		>* = nullptr
	>
	constexpr auto
	operator()(DistMatrix && a) const;
};

#else
struct columns_impl_el_matrix_1{};
struct columns_impl_el_matrix_2{};
struct columns_impl_el_dist_matrix{};
#endif
/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_COLUMNS_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                   \
		hbrs::mpl::detail::columns_impl_el_matrix_1{},                                                                 \
		hbrs::mpl::detail::columns_impl_el_matrix_2{},                                                                 \
		hbrs::mpl::detail::columns_impl_el_dist_matrix{}                                                               \
	)

#endif // !HBRS_MPL_FN_COLUMNS_FWD_ELEMENTAL_HPP
