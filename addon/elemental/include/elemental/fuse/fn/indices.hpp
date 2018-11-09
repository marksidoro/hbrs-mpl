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

#ifndef ELEMENTAL_FUSE_FN_INDICES_HPP
#define ELEMENTAL_FUSE_FN_INDICES_HPP

#include <elemental/config.hpp>
#include <elemental/dt/matrix.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/range/irange.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct indices_impl_smc_Matrix {
	template <
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	constexpr auto
	operator()(mpl::smc<Matrix, El::Int> const& a) const {
		return boost::irange(El::Int{0}, (*mpl::size)(a));
	}
};

struct indices_impl_smr_Matrix {
	template <
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	constexpr auto
	operator()(mpl::smr<Matrix, El::Int> const& a) const {
		return boost::irange(El::Int{0}, (*mpl::size)(a));
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_INDICES_IMPLS boost::hana::make_tuple(                                                       \
		elemental::detail::indices_impl_smc_Matrix{},                                                                  \
		elemental::detail::indices_impl_smr_Matrix{}                                                                   \
	)

#endif // !ELEMENTAL_FUSE_FN_INDICES_HPP
