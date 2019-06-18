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

#ifndef HBRS_MPL_FN_ITERATE_IMPL_STD_HPP
#define HBRS_MPL_FN_ITERATE_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/functional/iterate.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<typename F>
struct iterate_impl_helper {
	template<typename F_>
	constexpr
	helper(F_ && f, std::size_t n) : f_{HBRS_MPL_FWD(f)}, n_{n} {}
	
	constexpr
	helper(helper const&) = default;
	constexpr
	helper(helper &&) = default;
	
	constexpr helper&
	operator=(helper const&) = default;
	constexpr helper&
	operator=(helper &&) = default;
	
private:
	template<typename T>
	constexpr std::decay_t<T> /* no auto return type because decltype(f(t)) might not be equal to decltype(t) */
	iterate(T && t, std::size_t n) const {
		if (n==0) {
			return HBRS_MPL_FWD(t);
		} else {
			return iterate(f_(HBRS_MPL_FWD(t)), n - 1);
		}
	}
	
public:
	template<typename T>
	constexpr decltype(auto)
	operator()(T && t) const {
		return iterate(HBRS_MPL_FWD(t), n_);
	}
	
private:
	F f_;
	std::size_t n_;
};

template <typename F>
constexpr iterate_impl_helper<F>
iterate_impl::operator()(F && f, std::size_t n) const {
	return { HBRS_MPL_FWD(f), n };
}

template <typename F, std::size_t n>
constexpr decltype(auto)
iterate_impl::operator()(F && f, std::integral_constant<std::size_t, n>) const {
	return  hana::iterate<n>(HBRS_MPL_FWD(f));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_ITERATE_IMPL_STD_HPP
