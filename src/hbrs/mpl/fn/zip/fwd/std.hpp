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

#ifndef HBRS_MPL_FN_ZIP_FWD_STD_HPP
#define HBRS_MPL_FN_ZIP_FWD_STD_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/detail/tags/fwd.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/tuple.hpp>
#include <array>
#include <vector>
#include <tuple>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct zip_impl_std_array_vector_irange {
	template<
		typename S1,
		typename S2,
		typename std::enable_if_t<
			(
				std::is_same< hana::tag_of_t<S1>, hana::ext::std::array_tag>::value ||
				std::is_same< hana::tag_of_t<S1>, hana::ext::std::vector_tag>::value ||
				std::is_same< hana::tag_of_t<S1>, hana::ext::boost::integer_range_tag>::value
			) &&
			(
				std::is_same< hana::tag_of_t<S2>, hana::ext::std::array_tag>::value ||
				std::is_same< hana::tag_of_t<S2>, hana::ext::std::vector_tag>::value ||
				std::is_same< hana::tag_of_t<S2>, hana::ext::boost::integer_range_tag>::value
			)
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S1&& s1, S2&& s2) const;
};

struct zip_impl_std_tuple_vector {
	template<
		typename S1,
		typename S2,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<S1>, hana::ext::std::vector_tag>::value &&
			std::is_same< hana::tag_of_t<S2>, hana::ext::std::vector_tag>::value
		>* = nullptr
	>
	constexpr auto
	operator()(S1&& s1, S2&& s2) const;
};

struct zip_impl_std_tuple {
	template<
		typename S1,
		typename S2,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<S1>, hana::ext::std::tuple_tag>::value &&
			std::is_same< hana::tag_of_t<S2>, hana::ext::std::tuple_tag>::value &&
			std::tuple_size<std::remove_reference_t<S1>>::value == std::tuple_size<std::remove_reference_t<S2>>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(S1&& s1, S2&& s2) const;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_ZIP_IMPLS_STD boost::hana::make_tuple(                                                             \
		hbrs::mpl::detail::zip_impl_std_array_vector_irange{},                                                         \
		hbrs::mpl::detail::zip_impl_std_tuple_vector{},                                                                \
		hbrs::mpl::detail::zip_impl_std_tuple{}                                                                        \
	)

#endif // !HBRS_MPL_FN_ZIP_FWD_STD_HPP
