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

#ifndef HBRS_MPL_CORE_IS_ERROR_CAUSE_IMPL_HPP
#define HBRS_MPL_CORE_IS_ERROR_CAUSE_IMPL_HPP

#include "fwd.hpp"
#include <hbrs/mpl/dt/error.hpp>
#include <hbrs/mpl/dt/stack_frame.hpp>
#include <boost/hana/type.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

HBRS_MPL_DEF_CF2(is_error_cause)

template <typename CauseTag>
struct is_error_cause_impl<error_tag, CauseTag>{
	template <typename Error, typename Cause>
	static constexpr decltype(auto)
	apply(Error&& e, Cause&&) {
		return hana::type_c<std::decay_t< decltype(e.cause()) >> == hana::type_c<std::decay_t<Cause>>;
	}
};

template <typename CauseTag>
struct is_error_cause_impl<stack_frame_tag, CauseTag>{
	template <typename StackFrame, typename Cause>
	static constexpr decltype(auto)
	apply(StackFrame&& sf, Cause && c) {
		return is_error_cause(HBRS_MPL_FWD(sf).error(), HBRS_MPL_FWD(c));
	}
};

template <typename Tag, typename CauseTag, bool Condition>
struct is_error_cause_impl<Tag, CauseTag, hana::when<Condition>> : hana::default_ {
	
	template <typename T, typename Cause>
	static constexpr hana::false_
	apply(T&&, Cause&&) {
		return hana::false_c;
	}
	
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_IS_ERROR_CAUSE_IMPL_HPP
