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

#ifndef HBRS_MPL_FN_FOLD_LEFT_IMPL_STD_HPP
#define HBRS_MPL_FN_FOLD_LEFT_IMPL_STD_HPP

#include "../fwd/std.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/core/evaluate.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/functional/demux.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename S,
	typename I,
	typename F,
	typename std::enable_if_t<
		boost::mpl::if_c<
			std::is_same< hana::tag_of_t<S>, hana::ext::std::array_tag>::value,
			std::is_invocable<
				F&&,
				typename std::remove_reference_t<S>::value_type,
				typename std::remove_reference_t<S>::value_type
			>,
			std::false_type
		>::type::value
	>*
>
constexpr decltype(auto)
fold_left_impl_std_array::operator()(S&& s, I && i, F && f) const {
	return hana::fold_left(HBRS_MPL_FWD(s), HBRS_MPL_FWD(i), hana::demux(evaluate)(HBRS_MPL_FWD(f)));
}

template<
	typename S,
	typename F,
	typename std::enable_if_t<
		boost::mpl::if_c<
			std::is_same< hana::tag_of_t<S>, hana::ext::std::vector_tag>::value,
			std::is_invocable<
				F&&,
				typename std::remove_reference_t<S>::value_type,
				typename std::remove_reference_t<S>::value_type
			>,
			std::false_type
		>::type::value
	>*
>
constexpr decltype(auto)
fold_left_impl_std_vector::operator()(S && s, std::size_t i, F && f) const {
	return std::accumulate(
		s.begin(), 
		s.end(), 
		i,
		hana::demux(evaluate)(HBRS_MPL_FWD(f))
	);
}

template<
	typename S, 
	typename I, 
	typename F,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<S>, hana::ext::std::tuple_tag>::value 
	>*
>
constexpr decltype(auto)
fold_left_impl_std_tuple::operator()(S && s, I && i, F && f) const {
	return hana::fold_left(HBRS_MPL_FWD(s), HBRS_MPL_FWD(i), hana::demux(evaluate)(HBRS_MPL_FWD(f)));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_FOLD_LEFT_IMPL_STD_HPP
