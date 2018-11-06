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

#ifndef ELEMENTAL_FUSE_FN_DIVIDE_HPP
#define ELEMENTAL_FUSE_FN_DIVIDE_HPP

#include <elemental/config.hpp>
#include <El.hpp>
#include <elemental/fwd/dt/vector.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct divide_impl_vector_Scalar {
	template <
		typename Vector,
		typename Ring,
		typename std::enable_if_t< 
			(
				std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value ||
				std::is_same< hana::tag_of_t<Vector>, row_vector_tag >::value
			) &&
			std::is_arithmetic<Ring>::value
		>* = nullptr
	>
	auto
	operator()(Vector v, Ring const& b) const {
		//TODO: Move to member function? Or replace with transform call?
		typedef std::decay_t<Ring> _Ring_;
		El::Scale(_Ring_(1)/b, v.data());
		return v;
	}
};

struct divide_impl_Matrix_Scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<Ring> a, Ring const& b) const {
		El::Scale(Ring(1)/b, a);
		return a;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_DIVIDE_IMPLS boost::hana::make_tuple(                                                        \
		elemental::detail::divide_impl_vector_Scalar{},                                                                \
		elemental::detail::divide_impl_Matrix_Scalar{}                                                                 \
	)

#endif // !ELEMENTAL_FUSE_FN_DIVIDE_HPP
