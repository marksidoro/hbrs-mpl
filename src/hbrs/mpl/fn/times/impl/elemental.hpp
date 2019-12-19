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

template <typename Ring>
auto
times_impl_el_matrix_el_matrix::operator()(el_matrix<Ring> const& a, el_matrix<Ring> const& b) const {
	if ( (a.m() != b.m()) || (a.n() != b.n())) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< errinfo_el_matrix_sizes{{(*size)(a), (*size)(b)}}
		));
	}
	
	el_matrix<Ring> c{a.m(), a.n()};
	El::Hadamard(a.data(), b.data(), c.data());
	return c;
}

template <
	typename RingL,
	typename RingR,
	typename std::enable_if_t<
		!std::is_same_v<RingL, RingR> &&
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
	
	//TODO: Replace with faster algorithm, e.g. see Elemental/include/El/blas_like/level1/Hadamard.hpp
	EL_PARALLEL_FOR
	for(El::Int j = 0; j < a.n(); ++j) {
		EL_SIMD
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
		hana::tuple<
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
	
	BOOST_ASSERT(from.length() == lhs.n());
	
	El::DistMatrix<RingR, El::STAR, El::STAR, WrappingR> rhs_ = { from.data() }; // fetch complete vector to local process
	El::Matrix<RingR> & rhs_lcl = rhs_.Matrix();
	
	/* Code inspired by Elemental/include/El/blas_like/level1/Hadamard.hpp */
	
	El::Matrix<RingL> & lhs_lcl = lhs.data().Matrix();
	El::Int lhs_ldim = lhs_lcl.LDim();
	El::Int rhs_ldim = rhs_lcl.LDim();
	
	auto lhs_lcl_buf = lhs_lcl.Buffer();
	auto rhs_lcl_buf = rhs_lcl.LockedBuffer();
	
	EL_PARALLEL_FOR
	for(El::Int j = 0; j < lhs_lcl.Width(); ++j) {
		EL_SIMD
		for(El::Int i = 0; i < lhs_lcl.Height(); ++i) {
			std::size_t lhs_lcl_offset = i+j*lhs_ldim;
			std::size_t rhs_lcl_offset = rhs_.LocalRow(0) + rhs_.LocalCol(lhs.data().GlobalCol(j))*rhs_ldim;
			BOOST_ASSERT(lhs_lcl_offset < lhs_lcl.Width() * lhs_lcl.Height());
			BOOST_ASSERT(rhs_lcl_offset < rhs_lcl.Width() * rhs_lcl.Height());
			lhs_lcl_buf[lhs_lcl_offset] *= rhs_lcl_buf[rhs_lcl_offset];
		}
	}
	
	return lhs;
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
		hana::tuple<
			el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
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
	
	BOOST_ASSERT(from.length() == lhs.m());
	
	El::DistMatrix<RingR, El::STAR, El::STAR, WrappingR> rhs_ = { from.data() }; // fetch complete vector to local process
	El::Matrix<RingR> & rhs_lcl = rhs_.Matrix();
	
	/* Code inspired by Elemental/include/El/blas_like/level1/Hadamard.hpp */
	
	El::Matrix<RingL> & lhs_lcl = lhs.data().Matrix();
	El::Int lhs_ldim = lhs_lcl.LDim();
	El::Int rhs_ldim = rhs_lcl.LDim();
	
	auto lhs_lcl_buf = lhs_lcl.Buffer();
	auto rhs_lcl_buf = rhs_lcl.LockedBuffer();
	
	EL_PARALLEL_FOR
	for(El::Int j = 0; j < lhs_lcl.Width(); ++j) {
		EL_SIMD
		for(El::Int i = 0; i < lhs_lcl.Height(); ++i) {
			std::size_t lhs_lcl_offset = i+j*lhs_ldim;
			std::size_t rhs_lcl_offset = rhs_.LocalRow(lhs.data().GlobalRow(i)) + rhs_.LocalCol(0)*rhs_ldim;
			BOOST_ASSERT(lhs_lcl_offset < lhs_lcl.Width() * lhs_lcl.Height());
			BOOST_ASSERT(rhs_lcl_offset < rhs_lcl.Width() * rhs_lcl.Height());
			lhs_lcl_buf[lhs_lcl_offset] *= rhs_lcl_buf[rhs_lcl_offset];
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
times_impl_el_dist_matrix_expand_expr_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> const& lhs,
	expression<
		expand_t,
		hana::tuple<
			el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
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
		hana::tuple<
			el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
			matrix_size<El::Int, El::Int> const&
		>
	> rhs
) const {
	(*this)(lhs, rhs);
	return HBRS_MPL_FWD(lhs);
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
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> && lhs,
	expression<
		expand_t,
		hana::tuple<
			el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR>,
			matrix_size<El::Int, El::Int>
		>
	> rhs
) const {
	return (*this)(
		HBRS_MPL_FWD(lhs),
		make_expression(
			expand,
			hana::tuple<
				el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>{
				hana::at(rhs.operands(), hana::size_c<0>),
				hana::at(rhs.operands(), hana::size_c<1>)
			}
		)
	);
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
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> && lhs,
	expression<
		expand_t,
		hana::tuple<
			el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR>,
			matrix_size<El::Int, El::Int>
		>
	> rhs
) const {
	return (*this)(
		HBRS_MPL_FWD(lhs),
		make_expression(
			expand,
			hana::tuple<
				el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>{
				hana::at(rhs.operands(), hana::size_c<0>),
				hana::at(rhs.operands(), hana::size_c<1>)
			}
		)
	);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_TIMES_IMPL_ELEMENTAL_HPP
