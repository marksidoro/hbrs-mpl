/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_GIVENS_RESULT_IMPL_HPP
#define HBRS_MPL_DT_GIVENS_RESULT_IMPL_HPP

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename C, typename S>
struct givens_result {
	template<
		typename C_ = C,
		typename S_ = S,
		typename std::enable_if_t<
			std::is_default_constructible<C_>::value &&
			std::is_default_constructible<S_>::value
		>* = nullptr
	>
	constexpr
	givens_result() {}
	
	template<typename C_, typename S_>
	constexpr 
	givens_result(C_ && c, S_ && s) 
	: c_{HBRS_MPL_FWD(c)}, s_{HBRS_MPL_FWD(s)}
	{}
	
	constexpr 
	givens_result(givens_result const&) = default;
	constexpr 
	givens_result(givens_result &&) = default;
	
	constexpr givens_result&
	operator=(givens_result const&) = default;
	constexpr givens_result&
	operator=(givens_result &&) = default;
	
	constexpr decltype(auto)
	c() & { return (c_); };
	
	constexpr decltype(auto)
	c() const& { return (c_); };
	
	constexpr decltype(auto)
	c() && { return HBRS_MPL_FWD(c_); };
	
	constexpr decltype(auto)
	s() & { return (s_); };
	
	constexpr decltype(auto)
	s() const& { return (s_); };
	
	constexpr decltype(auto)
	s() && { return HBRS_MPL_FWD(s_); };
	
private:
	C c_;
	S s_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename C, typename S>
struct tag_of< hbrs::mpl::givens_result<C, S> > {
	using type = hbrs::mpl::givens_result_tag;
};

template <>
struct make_impl<hbrs::mpl::givens_result_tag> {
	template <typename C, typename S>
	static constexpr hbrs::mpl::givens_result<
		std::decay_t<C>,
		std::decay_t<S>
	>
	apply(C && c, S && s) {
		return {
			HBRS_MPL_FWD(c),
			HBRS_MPL_FWD(s)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_GIVENS_RESULT_IMPL_HPP
