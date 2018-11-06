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

#pragma once

#ifndef ELEMENTAL_FUSE_FN_MINUS_HPP
#define ELEMENTAL_FUSE_FN_MINUS_HPP

#include <elemental/config.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct minus_impl_Matrix_Matrix {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<Ring> a, El::Matrix<Ring> const& b) const {
		El::Axpy(Ring{-1.0},b,a);
		return a;
	}
};

struct minus_impl_Matrix_Scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<Ring> a, Ring const& b) const {
		auto ldim = a.LDim();
		for(El::Int j=0; j < a.Width(); ++j) {
			for(El::Int i=0; i < a.Height(); ++i) {
				a.Buffer()[i+j*ldim] -= b;
			}
		}
		return a;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_MINUS_IMPLS boost::hana::make_tuple(                                                         \
		elemental::detail::minus_impl_Matrix_Matrix{},                                                                 \
		elemental::detail::minus_impl_Matrix_Scalar{}                                                                  \
	)

#endif // !ELEMENTAL_FUSE_FN_MINUS_HPP
