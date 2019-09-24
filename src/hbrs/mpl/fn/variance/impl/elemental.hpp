/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_VARIANCE_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_VARIANCE_IMPL_ELEMENTAL_HPP

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
#include <hbrs/mpl/fn/minus.hpp>
#include <hbrs/mpl/fn/mean.hpp>
#include <hbrs/mpl/fn/transform.hpp>
#include <hbrs/mpl/fn/power.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <hbrs/mpl/fn/columns.hpp>

#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smrs.hpp>
#include <hbrs/mpl/dt/expression.hpp>
#include <hbrs/mpl/dt/exception.hpp>
#include <boost/assert.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <typename Sequence, typename Matrix, typename Weight>
auto
variance_impl_el(Sequence const& seq, Matrix const& mat, Weight w) {
	BOOST_ASSERT((w == 1) || (w == 0));

	auto && mu = (*mean)(seq);
	auto && cntr = (*minus)(mat, (*expand)(mu, size(mat)));
	
	auto square = [](auto x) { return power(x, 2); };
	auto && squared = (*transform)(cntr, square);
	auto && summed = (*sum)(columns(squared));
	auto biased_m = (*m)(size(mat))-1+w;
	
	return (*transform)(summed, [&biased_m](auto x) { return (*divide)(x, biased_m); });
}

template <
	typename Matrix,
	typename Weight,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Matrix>, el_matrix_tag > &&
		std::is_arithmetic_v<Weight>
	>*
>
auto
variance_impl_smcs_el_matrix::operator()(smcs<Matrix> const& seq, Weight w) const {
	return variance_impl_el(seq, seq.data() /* TODO: replace with matrix constructor call */, w);
}

template <
	typename DistMatrix,
	typename Weight,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag > &&
		std::is_arithmetic_v<Weight>
	>*
>
auto
variance_impl_el_dist_matrix_columns::operator()(
	expression<
		columns_t,
		std::tuple<DistMatrix>
	> const& expr,
	Weight w
) const {
	auto const& mat = hana::at_c<0>(expr.operands());
	return variance_impl_el(expr, mat, w);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_VARIANCE_IMPL_ELEMENTAL_HPP
