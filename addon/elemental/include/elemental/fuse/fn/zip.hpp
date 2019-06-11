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

#ifndef ELEMENTAL_FUSE_FN_ZIP_HPP
#define ELEMENTAL_FUSE_FN_ZIP_HPP

#include <elemental/config.hpp>
#include <elemental/dt/matrix.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/zas.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/range/irange.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct zip_impl_smc_matrix_integer_range {
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(mpl::smc<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
		return mpl::make_zas(a, b);
	}
	
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value &&
			!std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(mpl::smc<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
		typedef decltype(a.at(0)) Ring;
		typedef std::decay_t<Ring> _Ring_;
		
		matrix<_Ring_> c{a.length(), 1};
		for(El::Int i = 0; i < a.length(); ++i) {
			c.at({i, 0}) = a.at(i);
		}
		
		return mpl::make_zas(mpl::make_smc(std::move(c), 0), b);
	}
};

struct zip_impl_smr_matrix_integer_range {
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(mpl::smr<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
		return mpl::make_zas(a, b);
	}
	
	template <
		typename Matrix,
		typename Integer,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value &&
			!std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	constexpr auto
	operator()(mpl::smr<Matrix, El::Int> a, boost::integer_range<Integer> b) const {
		typedef decltype(a.at(0)) Ring;
		typedef std::decay_t<Ring> _Ring_;
		
		matrix<_Ring_> c{1, a.length()};
		for(El::Int i = 0; i < a.length(); ++i) {
			c.at({0, i}) = a.at(i);
		}
		
		return mpl::make_zas(mpl::make_smr(std::move(c), 0), b);
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_ZIP_IMPLS boost::hana::make_tuple(                                                           \
		elemental::detail::zip_impl_smc_matrix_integer_range{},                                                        \
		elemental::detail::zip_impl_smr_matrix_integer_range{}                                                         \
	)

#endif // !ELEMENTAL_FUSE_FN_ZIP_HPP
