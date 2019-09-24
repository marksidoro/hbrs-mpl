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

#ifndef HBRS_MPL_FN_MEAN_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_MEAN_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smc.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/sum.hpp>

#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smrs.hpp>
#include <hbrs/mpl/fn/columns.hpp>
#include <hbrs/mpl/dt/expression.hpp>
#include <hbrs/mpl/dt/exception.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Matrix,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
	>*
>
auto
mean_impl_smcs_el_matrix::operator()(smcs<Matrix> const& a) const {
	auto v = (*sum)(a);
	auto a_sz = (*size)(a.data());
	auto a_m = (*m)(a_sz);
	
	El::Scale(1.0/a_m, v.data());
	return v;
}

template <
	typename Matrix,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
	>*
>
auto
mean_impl_smrs_el_matrix::operator()(smrs<Matrix> const& a) const {
	auto v = (*sum)(a);
	auto a_sz = (*size)(a.data());
	auto a_n = (*n)(a_sz);
	
	El::Scale(1.0/a_n, v.data());
	return v;
}

template <typename Expression>
static auto
mean_el_dist_matrix_columns_impl(Expression && expr) {
	auto const& from = hana::at_c<0>(expr.operands());
	auto from_sz = (*size)(from);
	el_dist_row_vector sums = (*sum)(HBRS_MPL_FWD(expr));
	BOOST_ASSERT(sums.length() == from_sz.n());
	
	//TODO: Once sums is converted from "El::STAR, El::STAR" to "El::CIRC, El::CIRC" do only on sums_dmat.Grid().Rank() == 0!
	
	El::Scale(1.0/from_sz.m(), sums.data());
	return sums;
}

template <
	typename DistMatrix,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value
	>*
>
auto
mean_impl_el_dist_matrix_columns::operator()(
	expression<
		columns_t,
		hana::tuple<DistMatrix>
	> const& expr
) const {
	return mean_el_dist_matrix_columns_impl(expr);
}

template <
	typename DistMatrix,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value
	>*
>
auto
mean_impl_el_dist_matrix_columns::operator()(
	expression<
		columns_t,
		hana::tuple<DistMatrix>
	> && expr
) const {
	return mean_el_dist_matrix_columns_impl(HBRS_MPL_FWD(expr));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_MEAN_IMPL_ELEMENTAL_HPP
