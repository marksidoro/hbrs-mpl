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

#ifndef HBRS_MPL_FUSE_HBRS_MPL_FN_RECURSE_HPP
#define HBRS_MPL_FUSE_HBRS_MPL_FN_RECURSE_HPP

#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename Condition, 
	typename LastStep,
	typename RecursionStep
>
struct recurse_functor_t {
	constexpr
	recurse_functor_t(Condition const c, LastStep ls, RecursionStep rs) : c{c}, ls{ls}, rs{rs} {};
	
	template <typename Args>
	constexpr decltype(auto)
	eval(std::true_type, Args const& args) const {
		return hana::unpack(args, ls);
	}
	
	template <typename Args>
	constexpr decltype(auto)
	eval(std::false_type, Args const& args) const {
		auto vt = hana::unpack(args, rs);
		
		return eval( hana::unpack(vt, c), vt);
	}
	
	template <typename Args>
	constexpr decltype(auto)
	eval(hana::true_, Args const& args) const {
		return hana::unpack(args, ls);
	}
	
	template <typename Args>
	constexpr decltype(auto)
	eval(hana::false_, Args const& args) const {
		auto vt = hana::unpack(args, rs);
		
		return eval( hana::unpack(vt, c), vt);
	}
	
	template <typename Args>
	decltype(auto)
	eval(bool const& b, Args const& args) const {
		if (b) {
			return hana::unpack(args, ls);
		}
		
		auto vt = hana::unpack(args, rs);
		
		return eval(hana::unpack(vt, c), vt);
	}
	
	template <typename... Args>
	constexpr decltype(auto)
	operator()(Args&& ...args) const {
		return eval(
			c(args...),
			hana::tuple<Args...>{args...}
		);
	}
	
	Condition const c;
	LastStep ls;
	RecursionStep rs;
};

struct recurse_impl {
	
	template <
		typename Condition, 
		typename LastStep,
		typename RecursionStep
		//TODO: enable only if invokable...
	>
	constexpr decltype(auto)
	operator()(Condition && c, LastStep && ls, RecursionStep && rs) const {
		return recurse_functor_t<Condition, LastStep, RecursionStep>{c, ls, rs};
	}
	
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_HBRS_MPL_FN_RECURSE_IMPLS boost::hana::make_tuple(                                               \
		hbrs::mpl::detail::recurse_impl{}                                                                              \
	)


#endif // !HBRS_MPL_FUSE_HBRS_MPL_FN_RECURSE_HPP
