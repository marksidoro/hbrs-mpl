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

#ifndef ELEMENTAL_FUSE_FN_TIMES_HPP
#define ELEMENTAL_FUSE_FN_TIMES_HPP

#include <elemental/config.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/times.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>

#include <boost/mpl/void.hpp>
#include <elemental/dt/exception.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct times_impl_Matrix_Matrix {
	template <
		typename RingL,
		typename RingR,
		typename std::enable_if_t<
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<RingL> const& a, El::Matrix<RingR> const& b) const {
		using namespace hbrs::mpl;
		
		if ( (a.Height() != b.Height()) || (a.Width() != b.Width())) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrices_exception{}
				<< elemental::errinfo_matrix_sizes{{(*size)(a), (*size)(b)}}
			));
		}
		
		typedef std::common_type_t<RingL, RingR> Ring;
		
		El::Matrix<Ring> c{a.Height(), a.Width()};
		
		//TODO: Replace with faster algorithm, e.g. use parallelization
		for(El::Int j = 0; j < a.Width(); ++j) {
			for(El::Int i = 0; i < a.Height(); ++i) {
				(*at)(c, make_matrix_index(i,j)) = (*at)(a, make_matrix_index(i,j)) * (*at)(b, make_matrix_index(i,j));
			}
		}
		
		return c;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_TIMES_IMPLS boost::hana::make_tuple(                                                         \
		elemental::detail::times_impl_Matrix_Matrix{}                                                                  \
	)

#endif // !ELEMENTAL_FUSE_FN_TIMES_HPP
