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

#ifndef ELEMENTAL_FUSE_FN_TIMES_HPP
#define ELEMENTAL_FUSE_FN_TIMES_HPP

#include <hbrs/mpl/config.hpp>

#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
#include <hbrs/mpl/dt/el_vector/fwd.hpp>
#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
#include <hbrs/mpl/fn/expand/fwd.hpp>
#include <hbrs/mpl/dt/expression.hpp>
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

struct times_impl_matrix_matrix {
	template <
		typename RingL,
		typename RingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(matrix<RingL> const& a, matrix<RingR> const& b) const {
		using namespace hbrs::mpl;
		
		if ( (a.m() != b.m()) || (a.n() != b.n())) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrices_exception{}
				<< elemental::errinfo_matrix_sizes{{(*size)(a), (*size)(b)}}
			));
		}
		
		typedef std::common_type_t<RingL, RingR> Ring;
		
		matrix<Ring> c{a.m(), a.n()};
		
		//TODO: Replace with faster algorithm, e.g. use parallelization
		for(El::Int j = 0; j < a.n(); ++j) {
			for(El::Int i = 0; i < a.m(); ++i) {
				(*at)(c, make_matrix_index(i,j)) = (*at)(a, make_matrix_index(i,j)) * (*at)(b, make_matrix_index(i,j));
			}
		}
		
		return c;
	}
};

struct times_impl_dist_matrix_expand_expr_dist_matrix {
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
		auto const& from = hana::at_c<0>(rhs.operands());
		auto const& to_size = hana::at_c<1>(rhs.operands());
		
		using namespace hbrs::mpl;
		auto lhs_sz = (*size)(lhs);
		auto rhs_sz = to_size;
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

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_TIMES_IMPLS boost::hana::make_tuple(                                                         \
		elemental::detail::times_impl_matrix_matrix{},                                                                 \
		elemental::detail::times_impl_dist_matrix_expand_expr_dist_matrix{}                                            \
	)

#endif // !ELEMENTAL_FUSE_FN_TIMES_HPP
