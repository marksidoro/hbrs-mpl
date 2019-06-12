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

#ifndef HBRS_MPL_DETAIL_FORWARD_AND_COMPOSE_IMPL_HPP
#define HBRS_MPL_DETAIL_FORWARD_AND_COMPOSE_IMPL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct forward_and_compose_impl {
	template<typename F, typename G>
	struct helper_t {
		template<typename F_, typename G_>
		constexpr
		helper_t(F_ && f, G_ && g) : f_{HBRS_MPL_FWD(f)}, g_{HBRS_MPL_FWD(g)} {}
		
		template<typename X>
		constexpr decltype(auto)
		operator()(X && x) const {
			return HBRS_MPL_FWD(f_)(HBRS_MPL_FWD(g_)(HBRS_MPL_FWD(x)));
		}
		
		F f_;
		G g_;
	};
	
	template<typename F, typename G>
	constexpr helper_t<F&&, G&&>
	operator()(F && f, G && g) const {
		return { HBRS_MPL_FWD(f), HBRS_MPL_FWD(g) };
	}
};

constexpr forward_and_compose_impl forward_and_compose{};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_FORWARD_AND_COMPOSE_IMPL_HPP
