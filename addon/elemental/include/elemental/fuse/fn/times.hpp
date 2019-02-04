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

#pragma once

#ifndef ELEMENTAL_FUSE_FN_TIMES_HPP
#define ELEMENTAL_FUSE_FN_TIMES_HPP

#include <elemental/config.hpp>

#include <elemental/fwd/dt/matrix.hpp>
#include <elemental/fwd/dt/dist_matrix.hpp>
#include <elemental/fwd/dt/vector.hpp>
#include <elemental/fwd/dt/dist_vector.hpp>

#include <elemental/detail/Ring.hpp>
#include <elemental/detail/expand_expr.hpp>
#include <elemental/dt/exception.hpp>

#include <boost/hana/tuple.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/times.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>

#include <El.hpp>
#include <boost/mpl/void.hpp>

#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct times_impl_Matrix_Matrix {
	template <
		typename RingL,
		typename RingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<RingL> const& a, El::Matrix<RingR> const& b) const {
		using namespace hbrs::mpl;
		
		if ( (a.Height() != b.Height()) || (a.Width() != b.Width())) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrices_exception{}
				<< elemental::errinfo_matrix_sizes{{(*size)(a), (*size)(b)}}
			));
		}
		
		typedef std::common_type_t<RingL, RingR> Ring;
		
		El::Matrix<Ring> c{a.Height(), a.Width()};
		
		//TODO: Replace with faster algorithm, e.g. use parallelization
		for(El::Int j = 0; j < a.Width(); ++j) {
			for(El::Int i = 0; i < a.Height(); ++i) {
				(*at)(c, make_matrix_index(i,j)) = (*at)(a, make_matrix_index(i,j)) * (*at)(b, make_matrix_index(i,j));
			}
		}
		
		return c;
	}
};

//TODO: replace this hack! (like plus_impl_DistMatrix_expand_expr_DistMatrix)
struct times_impl_DistMatrix_expand_expr_DistMatrix {
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
	operator()(DistMatrixL const& lhs, expand_expr<dist_row_vector<DistMatrixR>> const& rhs) const {
		using namespace hbrs::mpl;
		auto lhs_sz = (*size)(lhs);
		auto rhs_sz = rhs.to_size;
		auto lhs_m = (*m)(lhs_sz);
		auto lhs_n = (*n)(lhs_sz);
		auto rhs_m = (*m)(rhs_sz);
		auto rhs_n = (*n)(rhs_sz);
		
		if ( (lhs_m != rhs_m) || (lhs_n != rhs_n)) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrices_exception{}
				<< elemental::errinfo_matrix_sizes{{lhs_sz, rhs_sz}}
			));
		}
		
		typedef Ring_t<std::decay_t<DistMatrixL>> RingL;
		typedef Ring_t<std::decay_t<DistMatrixR>> RingR;
		typedef std::decay_t<RingL> _RingL_;
		typedef std::decay_t<RingR> _RingR_;
		
		typedef std::common_type_t<_RingL_, _RingR_> _Ring_;
		El::DistMatrix<_Ring_> c{lhs.Grid()};
		c.Resize(lhs_m, lhs_n);
		
		//TODO: Replace with faster alg
		BOOST_ASSERT(rhs.from.data().Height() == 1);
		BOOST_ASSERT(rhs.from.data().Width() == lhs.Width());
		for(El::Int j = 0; j < lhs_n; ++j) {
			for(El::Int i = 0; i < lhs_m; ++i) {
				c.Set(i,j, lhs.Get(i,j) * rhs.from.data().Get(0,j));
			}
		}
		
		return c;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_TIMES_IMPLS boost::hana::make_tuple(                                                         \
		elemental::detail::times_impl_Matrix_Matrix{},                                                                 \
		elemental::detail::times_impl_DistMatrix_expand_expr_DistMatrix{}                                              \
	)

#endif // !ELEMENTAL_FUSE_FN_TIMES_HPP
