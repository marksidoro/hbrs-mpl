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

#ifndef HBRS_MPL_DT_ERROR_IMPL_HPP
#define HBRS_MPL_DT_ERROR_IMPL_HPP

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/no_context.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename Cause, typename Context>
struct error{
	template<
		typename Cause_, 
		typename Context_
	>
	constexpr 
	error(Cause_ && r, Context_ && c) 
	: 
	cause_{HBRS_MPL_FWD(r)}, 
	context_{HBRS_MPL_FWD(c)} 
	{}
	
	constexpr 
	error() : cause_{}, context_{} {}
	
	constexpr 
	error(error const&) = default;
	
	constexpr 
	error(error &&) = default;
	
	constexpr error&
	operator=(error const&) = default;
	
	constexpr error&
	operator=(error &&) = default;
	
	constexpr decltype(auto)
	cause() & { return (cause_); }
	
	constexpr decltype(auto)
	cause() const& { return (cause_); }
	
	constexpr decltype(auto)
	cause() && { return HBRS_MPL_FWD(cause_); }
	
	constexpr decltype(auto)
	context() & { return (context_); }
	
	constexpr decltype(auto)
	context() const& { return (context_); }
	
	constexpr decltype(auto)
	context() && { return HBRS_MPL_FWD(context_); }
	
	
private:
	Cause cause_;
	Context context_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Cause, typename Context>
struct tag_of< hbrs::mpl::error<Cause, Context> > {
	using type = hbrs::mpl::error_tag;
};

template <>
struct make_impl<hbrs::mpl::error_tag> {
	template <typename Cause, typename Context>
	static constexpr hbrs::mpl::error<
		std::decay_t<Cause>, 
		std::decay_t<Context>
	>
	apply(Cause && r, Context && c) {
		return {HBRS_MPL_FWD(r), HBRS_MPL_FWD(c)};
	}
	
	template <typename Cause>
	static constexpr hbrs::mpl::error<
		std::decay_t<Cause>, 
		hbrs::mpl::no_context
	>
	apply(Cause && r) {
		return {HBRS_MPL_FWD(r), hbrs::mpl::make_no_context()};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_ERROR_IMPL_HPP
