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

#ifndef HBRS_MPL_DT_BIDIAG_RESULT_IMPL_HPP
#define HBRS_MPL_DT_BIDIAG_RESULT_IMPL_HPP

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename U, typename B, typename V>
struct bidiag_result {
	template<
		typename U_ = U,
		typename B_ = B,
		typename V_ = V,
		typename std::enable_if_t<
			std::is_default_constructible<U_>::value &&
			std::is_default_constructible<B_>::value &&
			std::is_default_constructible<V_>::value
		>* = nullptr
	>
	constexpr
	bidiag_result() {}
	
	template<
		typename U_ = U,
		typename B_ = B,
		typename V_ = V,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<U, U_> &&
			detail::is_braces_constructible_v<B, B_> &&
			detail::is_braces_constructible_v<V, V_>
		>* = nullptr
	>
	constexpr 
	bidiag_result(U_ && u, B_ && b, V_ && v) 
	: u_{HBRS_MPL_FWD(u)}, b_{HBRS_MPL_FWD(b)}, v_{HBRS_MPL_FWD(v)}
	{}
	
	constexpr 
	bidiag_result(bidiag_result const&) = default;
	constexpr 
	bidiag_result(bidiag_result &&) = default;
	
	constexpr bidiag_result&
	operator=(bidiag_result const&) = default;
	constexpr bidiag_result&
	operator=(bidiag_result &&) = default;
	
	constexpr decltype(auto)
	u() & { return (u_); };
	
	constexpr decltype(auto)
	u() const& { return (u_); };
	
	constexpr decltype(auto)
	u() && { return HBRS_MPL_FWD(u_); };
	
	constexpr decltype(auto)
	b() & { return (b_); };
	
	constexpr decltype(auto)
	b() const& { return (b_); };
	
	constexpr decltype(auto)
	b() && { return HBRS_MPL_FWD(b_); };
	
	constexpr decltype(auto)
	v() & { return (v_); };
	
	constexpr decltype(auto)
	v() const& { return (v_); };
	
	constexpr decltype(auto)
	v() && { return HBRS_MPL_FWD(v_); };
	
private:
	U u_;
	B b_;
	V v_;
};

struct bidiag_u{};
struct bidiag_b{};
struct bidiag_v{};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename U, typename B, typename V>
struct tag_of< hbrs::mpl::bidiag_result<U, B, V> > {
	using type = hbrs::mpl::bidiag_result_tag;
};

template <>
struct make_impl<hbrs::mpl::bidiag_result_tag> {
	template <typename U, typename B, typename V>
	static constexpr hbrs::mpl::bidiag_result<
		std::decay_t<U>, 
		std::decay_t<B>, 
		std::decay_t<V>
	>
	apply(U && u, B && b, V && v) {
		return {
			HBRS_MPL_FWD(u),
			HBRS_MPL_FWD(b),
			HBRS_MPL_FWD(v)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_BIDIAG_RESULT_IMPL_HPP
