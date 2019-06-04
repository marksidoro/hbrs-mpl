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

#pragma once

#ifndef ELEMENTAL_FUSE_FN_MINUS_HPP
#define ELEMENTAL_FUSE_FN_MINUS_HPP

#include <elemental/config.hpp>
#include <elemental/dt/dist_matrix.hpp>
#include <elemental/dt/dist_vector.hpp>
#include <elemental/detail/Ring.hpp>
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

struct minus_impl_Matrix_Matrix {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value && !std::is_lvalue_reference<Ring>::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<Ring> a, El::Matrix<Ring> const& b) const {
		El::Axpy(Ring{-1},b,a);
		return a;
	}
};

//TODO: replace this hack! (like plus_impl_DistMatrix_expand_expr_DistMatrix)
struct minus_impl_DistMatrix_expand_expr_DistMatrix {
	template <
		typename DistMatrixL,
		typename DistMatrixR,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<DistMatrixL>, hana::ext::El::DistMatrix_tag >::value &&
			std::is_same< hana::tag_of_t<DistMatrixR>, hana::ext::El::DistMatrix_tag >::value &&
			std::is_same<
				Ring_t<std::decay_t<DistMatrixL>>,
				Ring_t<std::decay_t<DistMatrixR>>
			>::value &&
			!std::is_const< Ring_t<std::decay_t<DistMatrixL>> >::value
		>* = nullptr
	>
	auto
	operator()(
		DistMatrixL lhs,
		mpl::expression<
			mpl::expand_t,
			std::tuple<
				dist_row_vector<DistMatrixR> const&,
				mpl::matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const {
		auto const& from = hana::at_c<0>(rhs.operands());
		auto const& to_size = hana::at_c<1>(rhs.operands());
		typedef Ring_t<std::decay_t<DistMatrixL>> Ring;
		typedef std::decay_t<Ring> _Ring_;
		BOOST_ASSERT(lhs.Height() == to_size.m());
		BOOST_ASSERT(lhs.Width() == to_size.n());
		
		BOOST_ASSERT(from.data().Height() == 1);
		BOOST_ASSERT(from.data().Width() == lhs.Width());
		
		for(El::Int i = 0; i < lhs.Height(); ++i) {
			auto lhs_row = El::View(lhs, i, El::ALL);
			El::Axpy(_Ring_{-1}, from.data(), lhs_row);
		}
		
		return lhs;
	}
};

struct minus_impl_Matrix_Scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value && !std::is_lvalue_reference<Ring>::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<Ring> a, Ring const& b) const {
		auto ldim = a.LDim();
		for(El::Int j=0; j < a.Width(); ++j) {
			for(El::Int i=0; i < a.Height(); ++i) {
				a.Buffer()[i+j*ldim] -= b;
			}
		}
		return a;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_MINUS_IMPLS boost::hana::make_tuple(                                                         \
		elemental::detail::minus_impl_Matrix_Matrix{},                                                                 \
		elemental::detail::minus_impl_DistMatrix_expand_expr_DistMatrix{},                                             \
		elemental::detail::minus_impl_Matrix_Scalar{}                                                                  \
	)

#endif // !ELEMENTAL_FUSE_FN_MINUS_HPP
