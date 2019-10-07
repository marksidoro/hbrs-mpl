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

#ifndef HBRS_MPL_FN_EIG_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_EIG_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/eig_control/fwd.hpp>
#endif
#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct eig_impl_el_matrix {
	template <
		typename Ring,
		typename std::enable_if_t<
			!std::is_reference_v<Ring> &&
			!std::is_const_v<Ring> &&
			std::is_arithmetic_v<Ring>
		>* = nullptr
	>
	decltype(auto)
	operator()(
		el_matrix<Ring> const& a,
		eig_control<> const& ctrl
	) const;
};

struct eig_impl_el_dist_matrix {
	template<
		typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
		typename std::enable_if_t<
			!std::is_reference_v<Ring> &&
			!std::is_const_v<Ring> &&
			std::is_arithmetic_v<Ring>
		>* = nullptr
	>
	decltype(auto)
	operator()(
		el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
		eig_control<> const& ctrl
	) const;
};

#else
struct eig_impl_el_matrix {};
struct eig_impl_el_dist_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_EIG_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                       \
		hbrs::mpl::detail::eig_impl_el_matrix{},                                                                       \
		hbrs::mpl::detail::eig_impl_el_dist_matrix{}                                                                   \
	)

#endif // !HBRS_MPL_FN_EIG_FWD_ELEMENTAL_HPP
