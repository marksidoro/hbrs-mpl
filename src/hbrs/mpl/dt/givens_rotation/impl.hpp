/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_GIVENS_ROTATION_IMPL_HPP
#define HBRS_MPL_DT_GIVENS_ROTATION_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <array>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/type.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template<typename /* type of matrix entries */ Ring>
struct givens_rotation {
	givens_rotation(std::size_t i, std::size_t k, std::array<Ring, 2> theta)
	: i_{i}, k_{k}, theta_{theta} {}
	
	constexpr decltype(auto)
	i() & { return (i_); };
	
	constexpr decltype(auto)
	i() const& { return (i_); };
	
	constexpr decltype(auto)
	i() && { return HBRS_MPL_FWD(i_); };
	
	constexpr decltype(auto)
	k() & { return (k_); };
	
	constexpr decltype(auto)
	k() const& { return (k_); };
	
	constexpr decltype(auto)
	k() && { return HBRS_MPL_FWD(k_); };
	
	constexpr decltype(auto)
	theta() & { return (theta_); };
	
	constexpr decltype(auto)
	theta() const& { return (theta_); };
	
	constexpr decltype(auto)
	theta() && { return HBRS_MPL_FWD(theta_); };
	
private:
	std::size_t i_;
	std::size_t k_;
	std::array<Ring, 2> theta_;
};

template<typename Ring>
givens_rotation<Ring>
G(std::size_t i, std::size_t k, std::array<Ring, 2> theta) {
	return {i, k, theta};
}

namespace detail {

template<typename LHS, typename RHS>
struct givens_rotation_expression {
	template<typename LHS_ = LHS, typename RHS_ = RHS>
	constexpr
	givens_rotation_expression(LHS_ && lhs, RHS_ && rhs) : lhs_{HBRS_MPL_FWD(lhs)}, rhs_{HBRS_MPL_FWD(rhs)} {}
	
	constexpr decltype(auto)
	lhs() & { return (lhs_); };
	
	constexpr decltype(auto)
	lhs() const& { return (lhs_); };
	
	constexpr decltype(auto)
	lhs() && { return HBRS_MPL_FWD(lhs_); };
	
	constexpr decltype(auto)
	rhs() & { return (rhs_); };
	
	constexpr decltype(auto)
	rhs() const& { return (rhs_); };
	
	constexpr decltype(auto)
	rhs() && { return HBRS_MPL_FWD(rhs_); };
private:
	LHS lhs_;
	RHS rhs_;
};

/* namespace detail */ }

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Ring>
struct tag_of< hbrs::mpl::givens_rotation<Ring> > {
	using type = hbrs::mpl::givens_rotation_tag;
};

template <>
struct make_impl<hbrs::mpl::givens_rotation_tag> {
	template <
		typename Ring,
		typename std::enable_if_t<
			!std::is_const_v< std::remove_reference_t<Ring> >
		>* = nullptr
	>
	static hbrs::mpl::givens_rotation<Ring>
	apply(std::size_t i, std::size_t k, std::array<Ring, 2> theta) {
		return {i, k, theta};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_GIVENS_ROTATION_IMPL_HPP
