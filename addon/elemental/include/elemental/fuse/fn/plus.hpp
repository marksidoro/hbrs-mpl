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

#pragma once

#ifndef ELEMENTAL_FUSE_FN_PLUS_HPP
#define ELEMENTAL_FUSE_FN_PLUS_HPP

#include <elemental/config.hpp>
#include <elemental/dt/dist_matrix.hpp>
#include <elemental/dt/dist_vector.hpp>
#include <hbrs/mpl/fwd/fn/expand.hpp>
#include <hbrs/mpl/dt/expression.hpp>

#include <El.hpp>
#include <boost/assert.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct plus_impl_matrix_matrix {
	template <
		typename Ring,
		typename std::enable_if_t<
			!std::is_const<Ring>::value && !std::is_lvalue_reference<Ring>::value
		>* = nullptr
	>
	auto
	operator()(matrix<Ring> a, matrix<Ring> const& b) const {
		El::Axpy(Ring{1},b.data(),a.data());
		return a;
	}
};

struct plus_impl_dist_matrix_expand_expr_dist_matrix {
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			std::is_convertible_v<RingR, RingL>
		>* = nullptr
	>
	decltype(auto)
	operator()(
		dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> & lhs,
		mpl::expression<
			mpl::expand_t,
			std::tuple<
				dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				mpl::matrix_size<El::Int, El::Int> const&
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
			El::Axpy(_RingL_{1}, from.data(), lhs_row);
		}
		
		return lhs;
	}
	
	
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(
		dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> const& lhs,
		mpl::expression<
			mpl::expand_t,
			std::tuple<
				dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				mpl::matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const {
		//TODO: Implement more efficiently: no double assignment, first for copying and second for assignment above.
		return (*this)(dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL>{lhs}, rhs);
	}
	
	
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(
		dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> && lhs,
		mpl::expression<
			mpl::expand_t,
			std::tuple<
				dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				mpl::matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const {
		(*this)(lhs, rhs);
		return HBRS_MPL_FWD(lhs);
	}
};

struct plus_impl_matrix_scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value && !std::is_lvalue_reference<Ring>::value
		>* = nullptr
	>
	auto
	operator()(matrix<Ring> a, Ring const& b) const {
		auto ldim = a.data().LDim();
		for(El::Int j=0; j < a.n(); ++j) {
			for(El::Int i=0; i < a.m(); ++i) {
				a.data().Buffer()[i+j*ldim] += b;
			}
		}
		return a;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_PLUS_IMPLS boost::hana::make_tuple(                                                          \
		elemental::detail::plus_impl_matrix_matrix{},                                                                  \
		elemental::detail::plus_impl_dist_matrix_expand_expr_dist_matrix{},                                            \
		elemental::detail::plus_impl_matrix_scalar{}                                                                   \
	)

#endif // !ELEMENTAL_FUSE_FN_PLUS_HPP
