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

#ifndef HBRS_MPL_FUSE_STD_FN_SIZE_HPP
#define HBRS_MPL_FUSE_STD_FN_SIZE_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/function_object.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/length.hpp>
#include <array>
#include <vector>
#include <tuple>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct size_impl_std_array {
	template<
		typename S,
		typename std::enable_if_t< std::is_same< hana::tag_of_t<S>, hana::ext::std::array_tag>::value >* = nullptr
	>
	constexpr decltype(auto)
	operator()(S&& s) const {
		return hana::length(HBRS_MPL_FWD(s));
	}
};

HBRS_MPL_DEF_FO_TRY_METHOD(size_impl_std_vector, hana::ext::std::vector_tag, size)

struct size_impl_std_tuple {
	template<
		typename S,
		typename std::enable_if_t< std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value >* = nullptr
	>
	constexpr decltype(auto)
	operator()(S&& s) const {
		return typename std::tuple_size< std::remove_reference_t<S> >::type{};
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SIZE_IMPLS_STD boost::hana::make_tuple(                                                       \
		hbrs::mpl::detail::size_impl_std_array{},                                                                      \
		hbrs::mpl::detail::size_impl_std_vector{},                                                                     \
		hbrs::mpl::detail::size_impl_std_tuple{}                                                                       \
	)

#endif // !HBRS_MPL_FUSE_STD_FN_SIZE_HPP
