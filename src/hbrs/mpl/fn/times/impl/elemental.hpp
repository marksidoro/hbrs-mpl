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

#ifndef HBRS_MPL_FN_TIMES_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_TIMES_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/expression.hpp>
#include <hbrs/mpl/dt/exception.hpp>

#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/times.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <
	typename RingL,
	typename RingR,
	typename std::enable_if_t<
		boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
	>*
>
auto
times_impl_el_matrix_el_matrix::operator()(el_matrix<RingL> const& a, el_matrix<RingR> const& b) const {
	if ( (a.m() != b.m()) || (a.n() != b.n())) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< errinfo_el_matrix_sizes{{(*size)(a), (*size)(b)}}
		));
	}
	
	typedef std::common_type_t<RingL, RingR> Ring;
	
	el_matrix<Ring> c{a.m(), a.n()};
	
	//TODO: Replace with faster algorithm, e.g. use parallelization
	for(El::Int j = 0; j < a.n(); ++j) {
		for(El::Int i = 0; i < a.m(); ++i) {
			(*at)(c, make_matrix_index(i,j)) = (*at)(a, make_matrix_index(i,j)) * (*at)(b, make_matrix_index(i,j));
		}
	}
	
	return c;
}

template <
	typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
	typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
	typename std::enable_if_t<
		std::is_convertible_v<RingR, RingL>
	>*
>
decltype(auto)
times_impl_el_dist_matrix_expand_expr_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> & lhs,
	expression<
		expand_t,
		std::tuple<
			el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
			matrix_size<El::Int, El::Int> const&
		>
	> rhs
) const {
	auto const& from = hana::at_c<0>(rhs.operands());
	auto const& to_size = hana::at_c<1>(rhs.operands());
	
	auto lhs_sz = (*size)(lhs);
	auto rhs_sz = to_size;
	auto lhs_m = (*m)(lhs_sz);
	auto lhs_n = (*n)(lhs_sz);
	auto rhs_m = (*m)(rhs_sz);
	auto rhs_n = (*n)(rhs_sz);
	
	if ( (lhs_m != rhs_m) || (lhs_n != rhs_n)) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< errinfo_el_matrix_sizes{{lhs_sz, rhs_sz}}
		));
	}
	
	//TODO: Replace with faster alg
	BOOST_ASSERT(from.length() == lhs.n());
	
	for(El::Int j = 0; j < lhs_n; ++j) {
		for(El::Int i = 0; i < lhs_m; ++i) {
			lhs.data().Set(i,j, lhs.data().Get(i,j) * from.data().Get(0,j));
		}
	}
	
	return lhs;
}

template <
	typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
	typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
	typename std::enable_if_t<
		boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
	>*
>
auto
times_impl_el_dist_matrix_expand_expr_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> const& lhs,
	expression<
		expand_t,
		std::tuple<
			el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
			matrix_size<El::Int, El::Int> const&
		>
	> rhs
) const {
	//TODO: Implement more efficiently: no double assignment, first for copying and second for assignment above.
	return (*this)(el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL>{lhs}, rhs);
}


template <
	typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
	typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
	typename std::enable_if_t<
		boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
	>*
>
auto
times_impl_el_dist_matrix_expand_expr_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> && lhs,
	expression<
		expand_t,
		std::tuple<
			el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
			matrix_size<El::Int, El::Int> const&
		>
	> rhs
) const {
	(*this)(lhs, rhs);
	return HBRS_MPL_FWD(lhs);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_TIMES_IMPL_ELEMENTAL_HPP
