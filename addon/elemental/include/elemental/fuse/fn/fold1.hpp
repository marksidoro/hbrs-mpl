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

#ifndef ELEMENTAL_FUSE_FN_FOLD1_HPP
#define ELEMENTAL_FUSE_FN_FOLD1_HPP

#include <elemental/config.hpp>
#include <elemental/dt/matrix.hpp>
#include <hbrs/mpl/fn/fold1_left.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/zas.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/range/irange.hpp>
#include <type_traits>
#include <functional>
#include <vector>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct fold1_impl_zas_smc_Matrix_irange {
	template <
		typename Matrix,
		typename Integer,
		typename F,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(mpl::zas<mpl::smc<Matrix, El::Int>, boost::integer_range<Integer>> const& a, F && f) const {
		return (*mpl::fold1_left)(a, HBRS_MPL_FWD(f));
	}
};

struct fold1_impl_zas_smr_Matrix_irange {
	template <
		typename Matrix,
		typename Integer,
		typename F,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(mpl::zas<mpl::smr<Matrix, El::Int>, boost::integer_range<Integer>> const& a, F && f) const {
		return (*mpl::fold1_left)(a, HBRS_MPL_FWD(f));
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_FOLD1_IMPLS boost::hana::make_tuple(                                                         \
		elemental::detail::fold1_impl_zas_smc_Matrix_irange{},                                                         \
		elemental::detail::fold1_impl_zas_smr_Matrix_irange{}                                                          \
	)

#endif // !ELEMENTAL_FUSE_FN_FOLD1_HPP
