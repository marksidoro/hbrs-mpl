/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_CORE_EVALUATE_HPP
#define HBRS_MPL_CORE_EVALUATE_HPP

#include <hbrs/mpl/fwd/core/evaluate.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

// HBRS_MPL_DEF_CF2(evaluate)

template <
	typename Expression,
	typename Context
>
constexpr decltype(auto)
evaluate_t::operator()(Expression&& e, Context&& c) const {
	return evaluate_impl<
		hana::tag_of_t<Expression>,
		hana::tag_of_t<Context>
	>::apply( HBRS_MPL_FWD(e), HBRS_MPL_FWD(c) );
}

template <typename ExpressionTag, typename ContextTag, bool Condition>
struct evaluate_impl<ExpressionTag, ContextTag, hana::when<Condition>> : hana::default_ {
	template <
		typename Value, 
		typename Context,
		typename std::enable_if_t< std::is_rvalue_reference<Value&&>::value >* = nullptr
	>
	static constexpr auto
	/* When using decltype(auto) as return type then, if Value is not a reference (Value without & or &&), 
	 * this function returns a rvalue reference. But after calling this function Value is destroyed and 
	 * rvalue reference is dangling?!
	 * 
	 * Using auto (without decltype) as return type forces move construction and prevents perfect forwarding. 
	 * But this is okay because rvalue_reference_wrapper forces move construction, too.
	 */
	apply(Value && v, Context &&) {
		return HBRS_MPL_FWD(v);
	}
	
	template <
		typename Value, 
		typename Context,
		typename std::enable_if_t< std::is_lvalue_reference<Value&&>::value >* = nullptr
	>
	static constexpr decltype(auto)
	apply(Value && v, Context &&) {
		return HBRS_MPL_FWD(v);
	}
	
	template <
		typename Value, 
		typename Context,
		typename std::enable_if_t< !std::is_reference<Value&&>::value >* = nullptr
	>
	static constexpr auto
	apply(Value && v, Context &&) = delete;  //IS THIS ACTUALLY CALLABLE?!?
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_EVALUATE_HPP
