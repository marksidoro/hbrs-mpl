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

#ifndef HBRS_MPL_FN_TRANSPOSE_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_TRANSPOSE_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/scv.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Vector,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Vector>, srv_tag>::value
	>*
>
constexpr auto
transpose_impl_srv::operator()(Vector && v) const {
	return make_scv(HBRS_MPL_FWD(v));
}

template <
	typename Vector,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Vector>, scv_tag>::value
	>*
>
constexpr auto 
transpose_impl_scv::operator()(Vector && v) const {
	return make_srv(HBRS_MPL_FWD(v));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_TRANSPOSE_IMPL_HBRS_MPL_HPP
