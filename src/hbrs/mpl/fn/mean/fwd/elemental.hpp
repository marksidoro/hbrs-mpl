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

#ifndef HBRS_MPL_FN_MEAN_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_MEAN_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
#endif

#include <hbrs/mpl/dt/smr/fwd.hpp>
#include <hbrs/mpl/dt/smc/fwd.hpp>
#include <hbrs/mpl/dt/smcs/fwd.hpp>
#include <hbrs/mpl/dt/smrs/fwd.hpp>
#include <hbrs/mpl/fn/columns/fwd.hpp>
#include <hbrs/mpl/dt/expression/fwd.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct mean_impl_smcs_el_matrix {
	template <
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(smcs<Matrix> const& a) const;
};

struct mean_impl_smrs_el_matrix {
	template <
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(smrs<Matrix> const& a) const;
};

struct mean_impl_el_dist_matrix_columns {
	template <
		typename DistMatrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(
		expression<
			columns_t,
			std::tuple<DistMatrix>
		> const& expr
	) const;
	
	template <
		typename DistMatrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(
		expression<
			columns_t,
			std::tuple<DistMatrix>
		> && expr
	) const;
};

#else
struct mean_impl_smcs_el_matrix {};
struct mean_impl_smrs_el_matrix {};
struct mean_impl_el_dist_matrix_columns {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_MEAN_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                      \
		hbrs::mpl::detail::mean_impl_smcs_el_matrix{},                                                                 \
		hbrs::mpl::detail::mean_impl_smrs_el_matrix{},                                                                 \
		hbrs::mpl::detail::mean_impl_el_dist_matrix_columns{}                                                          \
	)

#endif // !HBRS_MPL_FN_MEAN_FWD_ELEMENTAL_HPP
