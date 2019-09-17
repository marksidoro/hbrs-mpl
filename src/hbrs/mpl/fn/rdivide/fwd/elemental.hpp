/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_RDIVIDE_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_RDIVIDE_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
	#include <El.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct rdivide_impl_scalar_el_vector {
	template <
		typename Scalar,
		typename Vector,
		typename std::enable_if_t< 
			std::is_arithmetic<Scalar>::value &&
			(
				std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value ||
				std::is_same< hana::tag_of_t<Vector>, el_row_vector_tag >::value
			)
		>* = nullptr
	>
	auto
	operator()(Scalar const& b, Vector v) const;
};

struct rdivide_impl_scalar_el_dist_vector {
	template <
		typename Scalar,
		typename DistVector,
		typename std::enable_if_t<
			(
				std::is_same_v< hana::tag_of_t<DistVector>, el_dist_column_vector_tag > ||
				std::is_same_v< hana::tag_of_t<DistVector>, el_dist_row_vector_tag >
			) &&
			std::is_arithmetic_v<Scalar>
		>* = nullptr
	>
	auto
	operator()(
		Scalar const& b,
		DistVector && v
	) const;
};

#else
struct rdivide_impl_scalar_el_vector{};
struct rdivide_impl_scalar_el_dist_vector{};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_RDIVIDE_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                   \
		hbrs::mpl::detail::rdivide_impl_scalar_el_vector{},                                                            \
		hbrs::mpl::detail::rdivide_impl_scalar_el_dist_vector{}                                                        \
	)

#endif // !HBRS_MPL_FN_RDIVIDE_FWD_ELEMENTAL_HPP
