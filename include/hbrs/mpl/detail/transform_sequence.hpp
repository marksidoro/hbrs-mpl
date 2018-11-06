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

#pragma once

#ifndef HBRS_MPL_DETAIL_TRANSFORM_SEQUENCE_HPP
#define HBRS_MPL_DETAIL_TRANSFORM_SEQUENCE_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/core/evaluate.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/append.hpp>
#include <boost/hana/integral_constant.hpp>
#include <type_traits>
#include <array>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <typename S, typename F, std::size_t... Is>
constexpr auto
transform_homogenous_sequence(S && s, F && f, std::index_sequence<Is...>) {
	static_assert(
		are_same<
			std::decay_t<decltype(
				evaluate( f((*at)(HBRS_MPL_FWD(s), Is)) )
			)>...
		>::value, "");
		
	typedef std::decay_t<decltype(
		evaluate( f( (*at)(HBRS_MPL_FWD(s), 0) ) )
	)>  Result;
	
	return std::array<Result, sizeof...(Is)>{
		evaluate(
			HBRS_MPL_FWD(f)((*at)(HBRS_MPL_FWD(s), Is))
		)...
	};
}

template <
	typename S,
	typename F,
	typename Size,
	typename std::enable_if_t<
		hana::IntegralConstant<Size>::value &&
		std::is_convertible<typename std::remove_reference_t<Size>::value_type, std::size_t>::value
	>* = nullptr
>
constexpr decltype(auto)
transform_homogenous_sequence(S && s, F && f, Size) {
	return transform_homogenous_sequence(
		HBRS_MPL_FWD(s), 
		HBRS_MPL_FWD(f), 
		std::make_index_sequence<(std::size_t)Size::value>{}
	);
}

template <typename Source, typename F, typename Target>
constexpr Target
transform_homogenous_sequence(Source && src, F && f, Target && tgt, std::size_t sz, std::size_t i) {
	if (i == sz) {
		return HBRS_MPL_FWD(tgt);
	} else {
		return transform_homogenous_sequence(
			src,
			f,
			(*append)(
				HBRS_MPL_FWD(tgt), 
				evaluate(
					f( 
						(*at)(src, i)
					)
				)
			),
			sz,
			i+1
		);
	}
}

template <typename Source, typename F, typename Target>
constexpr Target
transform_homogenous_sequence(Source && src, F && f, Target && tgt, std::size_t sz) {
	return transform_homogenous_sequence(
		HBRS_MPL_FWD(src),
		HBRS_MPL_FWD(f),
		HBRS_MPL_FWD(tgt),
		sz,
		0
	);
}

template <typename S, typename F>
constexpr auto
transform_homogenous_sequence(S && s, F && f, std::size_t sz) {
	typedef std::decay_t<decltype( 
		evaluate( f( (*at)(HBRS_MPL_FWD(s), 0) ) )
	)>  Result;
	
	std::vector<Result> tgt;
	tgt.reserve(sz);
	
	return transform_homogenous_sequence(
		HBRS_MPL_FWD(s),
		HBRS_MPL_FWD(f),
		tgt,
		sz
	);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_TRANSFORM_SEQUENCE_HPP
