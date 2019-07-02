/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_CORE_IS_ERROR_IMPL_HPP
#define HBRS_MPL_CORE_IS_ERROR_IMPL_HPP

#include "fwd.hpp"
#include <hbrs/mpl/dt/error.hpp>
#include <hbrs/mpl/dt/stack_frame.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

HBRS_MPL_DEF_CF1(is_error)

template <>
struct is_error_impl<error_tag>{
	template <typename Error>
	static constexpr hana::true_
	apply(Error&&) {
		return hana::true_c;
	}
};

template <>
struct is_error_impl<stack_frame_tag>{
	template <typename StackFrame>
	static constexpr hana::true_
	apply(StackFrame&& sf) {
		return is_error(HBRS_MPL_FWD(sf).error());
	}
};

template <typename Tag, bool Condition>
struct is_error_impl<Tag, hana::when<Condition>> : hana::default_ {
	
	template <typename T>
	static constexpr hana::false_
	apply(T &&) {
		return hana::false_c;
	}
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_IS_ERROR_IMPL_HPP
