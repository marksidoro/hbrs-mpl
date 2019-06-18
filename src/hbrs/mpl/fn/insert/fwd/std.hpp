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

#ifndef HBRS_MPL_FN_INSERT_FWD_STD_HPP
#define HBRS_MPL_FN_INSERT_FWD_STD_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/tags/fwd/std.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/assert.hpp>
#include <type_traits>
#include <map>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
	
struct insert_impl_std_map {
	template<
		typename Key, typename T, typename Compare, typename Allocator, 
		typename T1, typename T2,
		typename std::enable_if_t<
			(std::is_same<Key, T1>::value || std::is_convertible<T1, Key>::value) && 
			(std::is_same<T, T2>::value || std::is_convertible<T2, T>::value)
		>*
	>
	static constexpr hana::true_
	args_ok(std::map<Key, T, Compare, Allocator>, std::pair<T1, T2>);

	template<typename M, typename Pair>
	static constexpr hana::false_
	args_ok(M, Pair);

	template<
		typename M,
		typename Pair,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<M>, hana::ext::std::map_tag >::value &&
			std::is_same< hana::tag_of_t<Pair>, hana::ext::std::pair_tag >::value &&
			decltype(args_ok(std::declval<M>(), std::declval<Pair>())){}
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(M && m, Pair && p) const;
	
	template<
		typename M,
		typename Pair,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<M>, hana::ext::std::map_tag >::value &&
			std::is_same< hana::tag_of_t<Pair>, hana::ext::std::pair_tag >::value &&
			decltype(args_ok(std::declval<M>(), std::declval<Pair>())){}
		>* = nullptr
	>
	constexpr auto
	operator()(M const& m, Pair && p) const;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_INSERT_IMPLS_STD boost::hana::make_tuple(                                                     \
		hbrs::mpl::detail::insert_impl_std_map{}                                                                       \
	)

#endif // !HBRS_MPL_FN_INSERT_FWD_STD_HPP
