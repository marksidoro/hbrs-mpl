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

#ifndef HBRS_MPL_FUSE_STD_FN_FOLD1_HPP
#define HBRS_MPL_FUSE_STD_FN_FOLD1_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/fn/fold1_left.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct fold1_impl_std {
	template <
		typename Sequence,
		typename F,
		typename std::enable_if_t< 
			std::is_same<hana::tag_of_t<Sequence>, hana::ext::std::array_tag>::value ||
			std::is_same<hana::tag_of_t<Sequence>, hana::ext::std::vector_tag>::value ||
			std::is_same<hana::tag_of_t<Sequence>, hana::ext::std::tuple_tag>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(Sequence && s, F && f) const {
		return fold1_left(s, HBRS_MPL_FWD(f));
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_STD_FN_FOLD1_IMPLS boost::hana::make_tuple(                                                      \
		hbrs::mpl::detail::fold1_impl_std{}                                                                            \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_FOLD1_HPP
