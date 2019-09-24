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

#ifndef HBRS_MPL_FN_MINUS_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_MINUS_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/dt/expression.hpp>
#include <boost/assert.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Ring,
	typename std::enable_if_t< 
		!std::is_const< Ring >::value && !std::is_lvalue_reference<Ring>::value
	>*
>
auto
minus_impl_el_matrix_el_matrix::operator()(el_matrix<Ring> a, el_matrix<Ring> const& b) const {
	El::Axpy(Ring{-1},b.data(),a.data());
	return a;
}

template <
	typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
	typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
	typename std::enable_if_t<
		std::is_convertible_v<RingR, RingL>
	>*
>
decltype(auto)
minus_impl_el_dist_matrix_expand_expr_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> & lhs,
	expression<
		expand_t,
		hana::tuple<
			el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
			matrix_size<El::Int, El::Int> const&
		>
	> rhs
) const {
	typedef std::decay_t<RingL> _RingL_;
	
	auto const& from = hana::at_c<0>(rhs.operands());
	auto const& to_size = hana::at_c<1>(rhs.operands());
	
	BOOST_ASSERT(lhs.m() == to_size.m());
	BOOST_ASSERT(lhs.n() == to_size.n());
	BOOST_ASSERT(from.length() == lhs.n());
	
	for(El::Int i = 0; i < lhs.m(); ++i) {
		auto lhs_row = El::View(lhs.data(), i, El::ALL);
		El::Axpy(_RingL_{-1}, from.data(), lhs_row);
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
minus_impl_el_dist_matrix_expand_expr_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> const& lhs,
	expression<
		expand_t,
		hana::tuple<
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
minus_impl_el_dist_matrix_expand_expr_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> && lhs,
	expression<
		expand_t,
		hana::tuple<
			el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
			matrix_size<El::Int, El::Int> const&
		>
	> rhs
) const {
	(*this)(lhs, rhs);
	return HBRS_MPL_FWD(lhs);
}


template <
	typename Ring,
	typename std::enable_if_t< 
		!std::is_const< Ring >::value && !std::is_lvalue_reference<Ring>::value
	>*
>
auto
minus_impl_el_matrix_scalar::operator()(el_matrix<Ring> a, Ring const& b) const {
	auto ldim = a.data().LDim();
	for(El::Int j=0; j < a.n(); ++j) {
		for(El::Int i=0; i < a.m(); ++i) {
			a.data().Buffer()[i+j*ldim] -= b;
		}
	}
	return a;
}


/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_MINUS_IMPL_ELEMENTAL_HPP
