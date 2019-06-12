/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DETAIL_TRANSFORM_AND_FORWARD_IMPL_HPP
#define HBRS_MPL_DETAIL_TRANSFORM_AND_FORWARD_IMPL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/forward_as_tuple.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/unpack.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct transform_and_forward_variadic_t {
	template <typename F, typename... Ts>
	constexpr decltype(auto)
	operator()(F&& f, Ts&&... ts) const {
		return forward_as_tuple( HBRS_MPL_FWD(f)(HBRS_MPL_FWD(ts))... );
	}
};

constexpr transform_and_forward_variadic_t transform_and_forward_variadic{};

struct transform_and_forward_t {
	template <typename S, typename F>
	constexpr decltype(auto)
	operator()(S&& s, F&& f) const {
		return hana::unpack(
			HBRS_MPL_FWD(s), 
			hana::partial(transform_and_forward_variadic, HBRS_MPL_FWD(f))
		);
	}
};

constexpr transform_and_forward_t transform_and_forward{};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_TRANSFORM_AND_FORWARD_IMPL_HPP
