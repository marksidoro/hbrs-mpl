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

#ifndef HBRS_MPL_FN_COLUMNS_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_COLUMNS_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/smcs.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Matrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, sm_tag >::value ||
		std::is_same< hana::tag_of_t<Matrix>, rtsam_tag >::value ||
		std::is_same< hana::tag_of_t<Matrix>, ctsam_tag >::value
	>*
>
constexpr auto
columns_impl::operator()(Matrix && a) const {
	return smcs<Matrix>{HBRS_MPL_FWD(a)};
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_COLUMNS_IMPL_HBRS_MPL_HPP
