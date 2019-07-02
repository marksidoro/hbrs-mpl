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

#ifndef HBRS_MPL_DT_STACK_FRAME_IMPL_HPP
#define HBRS_MPL_DT_STACK_FRAME_IMPL_HPP

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename Error, typename Function, typename Arguments>
struct stack_frame{
	template<typename Error_, typename Function_, typename Arguments_>
	constexpr 
	stack_frame(Error_ && e, Function_ && f, Arguments_&& args) 
	: error_{HBRS_MPL_FWD(e)}, function_{HBRS_MPL_FWD(f)}, arguments_{HBRS_MPL_FWD(args)} {}
	
	constexpr 
	stack_frame(stack_frame const&) = default;
	
	constexpr 
	stack_frame(stack_frame &&) = default;
	
	constexpr stack_frame&
	operator=(stack_frame const&) = default;
	constexpr stack_frame&
	operator=(stack_frame &&) = default;
	
	constexpr decltype(auto)
	error() & { return (error_); }
	
	constexpr decltype(auto)
	error() const& { return (error_); }
	
	constexpr decltype(auto)
	error() && { return HBRS_MPL_FWD(error_); }
	
	constexpr decltype(auto)
	function() & { return (function_); }
	
	constexpr decltype(auto)
	function() const& { return (function_); }
	
	constexpr Function &&
	function() && { return HBRS_MPL_FWD(function_); }
	
	constexpr decltype(auto)
	arguments() & { return (arguments_); }
	
	constexpr decltype(auto)
	arguments() const& { return (arguments_); }
	
	constexpr decltype(auto)
	arguments() && { return HBRS_MPL_FWD(arguments_); }
	
private:
	Error error_;
	Function function_;
	Arguments arguments_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Error, typename Function, typename Arguments>
struct tag_of< hbrs::mpl::stack_frame<Error, Function, Arguments> > {
	using type = hbrs::mpl::stack_frame_tag;
};

template <>
struct make_impl<hbrs::mpl::stack_frame_tag> {
	template <typename Error, typename Function, typename Arguments>
	static constexpr hbrs::mpl::stack_frame<
		std::decay_t<Error>,
		std::decay_t<Function>,
		std::decay_t<Arguments>
	>
	apply(Error && e, Function && f, Arguments&& args) {
		return {HBRS_MPL_FWD(e), HBRS_MPL_FWD(f), HBRS_MPL_FWD(args)};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_STACK_FRAME_IMPL_HPP
