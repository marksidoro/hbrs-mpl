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

#ifndef HBRS_MPL_FN_TIMES_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_TIMES_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
	#include <hbrs/mpl/fn/expand/fwd.hpp>
	#include <hbrs/mpl/dt/expression/fwd.hpp>
	#include <boost/mpl/void.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct times_impl_el_matrix_el_matrix {
	template <
		typename RingL,
		typename RingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(el_matrix<RingL> const& a, el_matrix<RingR> const& b) const;
};

struct times_impl_el_dist_matrix_expand_expr_el_dist_matrix {
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			std::is_convertible_v<RingR, RingL>
		>* = nullptr
	>
	decltype(auto)
	operator()(
		el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> & lhs,
		expression<
			expand_t,
			std::tuple<
				el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const;
	
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			std::is_convertible_v<RingR, RingL>
		>* = nullptr
	>
	decltype(auto)
	operator()(
		el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> & lhs,
		expression<
			expand_t,
			std::tuple<
				el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const;
	
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(
		el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> const& lhs,
		expression<
			expand_t,
			std::tuple<
				el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const;
	
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(
		el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> const& lhs,
		expression<
			expand_t,
			std::tuple<
				el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const;
	
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(
		el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> && lhs,
		expression<
			expand_t,
			std::tuple<
				el_dist_row_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const;
	
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap WrappingL,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR, El::DistWrap WrappingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(
		el_dist_matrix<RingL, ColumnwiseL, RowwiseL, WrappingL> && lhs,
		expression<
			expand_t,
			std::tuple<
				el_dist_column_vector<RingR, ColumnwiseR, RowwiseR, WrappingR> const&,
				matrix_size<El::Int, El::Int> const&
			>
		> rhs
	) const;
};

#else
struct times_impl_el_matrix_el_matrix {};
struct times_impl_el_dist_matrix_expand_expr_el_dist_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_TIMES_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                     \
		hbrs::mpl::detail::times_impl_el_matrix_el_matrix{},                                                           \
		hbrs::mpl::detail::times_impl_el_dist_matrix_expand_expr_el_dist_matrix{}                                      \
	)

#endif // !HBRS_MPL_FN_TIMES_FWD_ELEMENTAL_HPP
