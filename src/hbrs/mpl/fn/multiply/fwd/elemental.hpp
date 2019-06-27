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

#ifndef HBRS_MPL_FN_MULTIPLY_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_MULTIPLY_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/scv/fwd.hpp>
	#include <boost/mpl/if.hpp>
	#include <boost/mpl/void.hpp>
	#include <boost/hana/ext/std/vector.hpp>
	#include <type_traits>
#endif

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct multiply_impl_el_matrix_el_matrix {
	template <
		typename RingL,
		typename RingR,
		typename std::enable_if_t<
			!std::is_same<RingL, RingR>::value &&
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	decltype(auto)
	operator()(el_matrix<RingL> const& a, el_matrix<RingR> const& b) const;
	
	template <typename Ring>
	decltype(auto)
	operator()(el_matrix<Ring> const& a, el_matrix<Ring> const& b) const;
};

struct multiply_impl_el_dist_matrix_el_dist_matrix {
	template <
		typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap Wrapping,
		typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR,
		typename std::enable_if_t<
			std::is_same_v<RingL, RingR> ||
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		el_dist_matrix<RingL, ColumnwiseL, RowwiseL, Wrapping> const& a,
		el_dist_matrix<RingR, ColumnwiseR, RowwiseR, Wrapping> const& b
	) const;
};

struct multiply_impl_el_matrix_scv_vector {
	template <
		typename MatrixRing,
		typename Sequence,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Sequence>, hana::ext::std::vector_tag >::value
			//TODO: Check that MatrixRing and VectorRing (see below) are compatible!
		>* = nullptr
	>
	auto
	operator()(el_matrix<MatrixRing> const& a, scv<Sequence> const& b) const;
};

struct multiply_impl_el_matrix_scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(el_matrix<Ring> a, Ring const& b) const;
};

#else
struct multiply_impl_el_matrix_el_matrix {};
struct multiply_impl_el_dist_matrix_el_dist_matrix {};
struct multiply_impl_el_matrix_scv_vector {};
struct multiply_impl_el_matrix_scalar {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_MULTIPLY_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                  \
		hbrs::mpl::detail::multiply_impl_el_matrix_el_matrix{},                                                        \
		hbrs::mpl::detail::multiply_impl_el_matrix_scv_vector{},                                                       \
		hbrs::mpl::detail::multiply_impl_el_matrix_scalar{},                                                           \
		hbrs::mpl::detail::multiply_impl_el_dist_matrix_el_dist_matrix{}                                               \
	)

#endif // !HBRS_MPL_FN_MULTIPLY_FWD_ELEMENTAL_HPP
