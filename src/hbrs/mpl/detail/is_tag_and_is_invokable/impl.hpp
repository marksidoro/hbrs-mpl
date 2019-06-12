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

#ifndef HBRS_MPL_DETAIL_IS_TAG_AND_IS_INVOKABLE_HPP
#define HBRS_MPL_DETAIL_IS_TAG_AND_IS_INVOKABLE_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/type.hpp>
#include <boost/mpl/if.hpp>
#include <tuple>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<typename F, typename Object, typename ObjectTag, typename... Args>
struct is_tag_and_is_invokable_trait {
	using type = typename 
		boost::mpl::if_c<
			std::is_same< hana::tag_of_t<Object>, ObjectTag >::value,
			std::is_invocable<F&&, Object&&, Args&&...>,
			std::false_type
		>::type;
	static constexpr bool value = type::value;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_IS_TAG_AND_IS_INVOKABLE_HPP
