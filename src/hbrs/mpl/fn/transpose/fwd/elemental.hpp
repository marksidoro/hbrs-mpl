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

#ifndef HBRS_MPL_FN_TRANSPOSE_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_TRANSPOSE_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct transpose_impl_el_matrix {
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value 
		>* = nullptr
	>
	auto
	operator()(Matrix && m) const;
};

struct transpose_impl_el_vector {
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<Vector>, el_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<Vector>, el_row_vector_tag >
		>* = nullptr
	>
	auto
	operator()(Vector && v) const;
};

struct transpose_impl_el_dist_matrix {
	template<
		typename DistMatrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value 
		>* = nullptr
	>
	auto
	operator()(DistMatrix && m) const;
};

struct transpose_impl_el_dist_vector {
	template<
		typename DistVector,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<DistVector>, el_dist_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<DistVector>, el_dist_row_vector_tag >
		>* = nullptr
	>
	auto
	operator()(DistVector && v) const;
};

#else
struct transpose_impl_el_matrix {};
struct transpose_impl_el_vector{};
struct transpose_impl_el_dist_matrix {};
struct transpose_impl_el_dist_vector{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_TRANSPOSE_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                 \
		hbrs::mpl::detail::transpose_impl_el_matrix{},                                                                 \
		hbrs::mpl::detail::transpose_impl_el_vector{},                                                                 \
		hbrs::mpl::detail::transpose_impl_el_dist_matrix{},                                                            \
		hbrs::mpl::detail::transpose_impl_el_dist_vector{}                                                             \
	)

#endif // !HBRS_MPL_FN_TRANSPOSE_FWD_ELEMENTAL_HPP
