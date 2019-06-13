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

#ifndef HBRS_MPL_FUSE_BOOST_HANA_FN_AT_HPP
#define HBRS_MPL_FUSE_BOOST_HANA_FN_AT_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/fn/apply_at.hpp>
#include <boost/hana/at.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <hbrs/mpl/detail/is_homogenous_container.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct at_impl_hana_tuple_ic {
	
	template<
		typename S,
		typename Where,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<S>, hana::tuple_tag >::value && 
			hana::IntegralConstant<Where>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S && s, Where && w) const {
		return hana::at(HBRS_MPL_FWD(s), HBRS_MPL_FWD(w));
	}
	
};

struct at_impl_hana_tuple_hmg_int_entry {
	template<
		typename S, 
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<S>, hana::tuple_tag >::value &&
			detail::is_homogenous_container_v<S>
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S&& s, std::size_t w) const {
		return apply_at(hana::id, HBRS_MPL_FWD(s), w);
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_AT_IMPLS_BOOST_HANA boost::hana::make_tuple(                                                  \
		hbrs::mpl::detail::at_impl_hana_tuple_ic{},                                                                    \
		hbrs::mpl::detail::at_impl_hana_tuple_hmg_int_entry{}                                                          \
	)

#endif // !HBRS_MPL_FUSE_BOOST_HANA_FN_AT_HPP
