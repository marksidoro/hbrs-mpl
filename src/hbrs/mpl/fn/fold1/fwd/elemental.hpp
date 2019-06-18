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

#ifndef HBRS_MPL_FN_FOLD1_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_FOLD1_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
#endif

#include <hbrs/mpl/dt/smc/fwd.hpp>
#include <hbrs/mpl/dt/smr/fwd.hpp>
#include <hbrs/mpl/dt/zas/fwd.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/range/irange.hpp>
#include <type_traits>
#include <functional>
#include <vector>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct fold1_impl_zas_smc_el_matrix_irange {
	template <
		typename Matrix,
		typename Integer,
		typename F,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(mpl::zas<mpl::smc<Matrix, El::Int>, boost::integer_range<Integer>> const& a, F && f) const;
};

struct fold1_impl_zas_smr_el_matrix_irange {
	template <
		typename Matrix,
		typename Integer,
		typename F,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(mpl::zas<mpl::smr<Matrix, El::Int>, boost::integer_range<Integer>> const& a, F && f) const;
};

#else
struct fold1_impl_zas_smc_el_matrix_irange {};
struct fold1_impl_zas_smr_el_matrix_irange {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_FOLD1_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                     \
		hbrs::mpl::detail::fold1_impl_zas_smc_el_matrix_irange{},                                                      \
		hbrs::mpl::detail::fold1_impl_zas_smr_el_matrix_irange{}                                                       \
	)

#endif // !HBRS_MPL_FN_FOLD1_FWD_ELEMENTAL_HPP
