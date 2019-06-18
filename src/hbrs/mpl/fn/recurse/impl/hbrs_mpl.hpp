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

#ifndef HBRS_MPL_FN_RECURSE_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_RECURSE_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/unpack.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<typename Condition, typename LastStep, typename RecursionStep>
struct recurse_functor_t {
	
	template<typename Condition_, typename LastStep_, typename RecursionStep_>
	constexpr
	recurse_functor_t(Condition_ && c, LastStep_ && ls, RecursionStep_ && rs)
	: c{HBRS_MPL_FWD(c)}, ls{HBRS_MPL_FWD(ls)}, rs{HBRS_MPL_FWD(rs)} {}
	
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
	
	Condition c;
	LastStep ls;
	RecursionStep rs;
};

template <
	typename Condition,
	typename LastStep,
	typename RecursionStep
	//TODO: enable only if invokable...
>
constexpr recurse_functor_t<
	std::decay_t<Condition>,
	std::decay_t<LastStep>,
	std::decay_t<RecursionStep>
>
recurse_impl::operator()(Condition && c, LastStep && ls, RecursionStep && rs) const {
	return {HBRS_MPL_FWD(c), HBRS_MPL_FWD(ls), HBRS_MPL_FWD(rs)};
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_RECURSE_IMPL_HBRS_MPL_HPP
