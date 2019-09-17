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

#ifndef HBRS_MPL_FN_DIVIDE_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_DIVIDE_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
	#include <El.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
struct divide_impl_el_vector_scalar {
	template <
		typename Vector,
		typename Scalar,
		typename std::enable_if_t< 
			(
				std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value ||
				std::is_same< hana::tag_of_t<Vector>, el_row_vector_tag >::value
			) &&
			std::is_arithmetic<Scalar>::value
		>* = nullptr
	>
	auto
	operator()(Vector v, Scalar const& b) const;
};

struct divide_impl_el_dist_vector_scalar {
	template <
		typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
		typename Scalar,
		typename std::enable_if_t<
			std::is_arithmetic_v<Scalar>
		>* = nullptr
	>
	auto
	operator()(
		el_dist_column_vector<Ring, Columnwise, Rowwise, Wrapping> && v,
		Scalar const& b
	) const;
};

struct divide_impl_el_matrix_scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(el_matrix<Ring> a, Ring const& b) const;
};

struct divide_impl_el_complex_scalar {
	template <
		typename Scalar,
		typename std::enable_if_t<
			std::is_arithmetic_v<Scalar>
		>* = nullptr
	>
	auto
	operator()(El::Complex<Scalar> const& a, Scalar const& b) const;
};

#else
struct divide_impl_el_vector_scalar {};
struct divide_impl_el_dist_vector_scalar {};
struct divide_impl_el_matrix_scalar {};
struct divide_impl_el_complex_scalar {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_DIVIDE_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                    \
		hbrs::mpl::detail::divide_impl_el_vector_scalar{},                                                             \
		hbrs::mpl::detail::divide_impl_el_dist_vector_scalar{},                                                        \
		hbrs::mpl::detail::divide_impl_el_matrix_scalar{},                                                             \
		hbrs::mpl::detail::divide_impl_el_complex_scalar{}                                                             \
	)

#endif // !HBRS_MPL_FN_DIVIDE_FWD_ELEMENTAL_HPP
