/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DETAIL_FOLD1_LEFT_HPP
#define HBRS_MPL_DETAIL_FOLD1_LEFT_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/exception.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/one.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/plus.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct fold1_left_t {
	template <
		typename Sequence,
		typename Value,
		typename Index,
		typename F,
		typename T,
		typename std::enable_if_t< 
			std::remove_reference_t<Index>::value+1 == std::remove_reference_t<T>::value
		>* = nullptr
	>
	static constexpr decltype(auto)
	recursive_call(Sequence && s, Value && v, Index && j, F && f, T sz) {
		return HBRS_MPL_FWD(f)(HBRS_MPL_FWD(v), (*at)(HBRS_MPL_FWD(s), j));
	}
	
	template <
		typename Sequence,
		typename Value,
		typename Index,
		typename F,
		typename T,
		typename std::enable_if_t< 
			std::remove_reference_t<Index>::value+1 < std::remove_reference_t<T>::value
		>* = nullptr
	>
	static constexpr decltype(auto)
	recursive_call(Sequence && s, Value && v, Index && j, F && f, T sz) {
		return recursive_call(
			s,
			f(HBRS_MPL_FWD(v), (*at)(s, j)),
			hana::plus(j, hana::one<hana::tag_of_t<std::decay_t<T>>>()),
			f,
			sz
		);
	}

	template <
		typename Sequence,
		typename F,
		typename T,
		T sz,
		typename std::enable_if_t< 
			(sz >= T{2})
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(Sequence && s, F && f, std::integral_constant<T, sz>) const {
		return recursive_call(
			s,
			(*at)(s, std::integral_constant<T, 0>{}),
			std::integral_constant<T, 1>{},
			HBRS_MPL_FWD(f),
			std::integral_constant<T, sz>{}
		);
	}

	template <
		typename Sequence,
		typename F,
		typename T,
		T sz,
		typename std::enable_if_t< 
			(sz >= T{2})
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(Sequence && s, F && f, hana::integral_constant<T, sz>) const {
		return recursive_call(
			s,
			(*at)(s, hana::integral_constant<T, 0>{}),
			hana::integral_constant<T, 1>{},
			HBRS_MPL_FWD(f),
			hana::integral_constant<T, sz>{}
		);
	}
	
	/* Tail-recursive function equivalent to: 
	 *  auto e = (*at)(s, T{0});
	 *  for(T i = 1; i < v; ++i) {
	 *   e = HBRS_MPL_FWD(f)(e, (*at)(s, i));
	 *  }
	 *  return e;
	 */
	template <
		typename Sequence,
		typename Value,
		typename Index,
		typename F,
		typename T,
		typename std::enable_if_t< 
			std::is_integral<T>::value
		>* = nullptr
	>
	static constexpr decltype(auto)
	tail_call(Sequence && s, Value && v, Index && j, F && f, T sz) {
		if (j+1 == sz) {
			return HBRS_MPL_FWD(f)(HBRS_MPL_FWD(v), (*at)(HBRS_MPL_FWD(s), j));
		} else {
			return tail_call(
				s,
				f(HBRS_MPL_FWD(v), (*at)(s, j)),
				j+1,
				f,
				sz
			);
		}
	}

	template <
		typename Sequence,
		typename F,
		typename T,
		typename std::enable_if_t< 
			std::is_arithmetic< T >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	decltype(auto)
	operator()(Sequence && s, F && f, T sz) const {
		if (sz < 2) {
			BOOST_THROW_EXCEPTION((
				incompatible_sequence_exception{} 
				<< errinfo_sequence_size{ (std::size_t)sz }
			));
		}
		
		return tail_call(
			s,
			(*at)(s, T{0}),
			T{1},
			HBRS_MPL_FWD(f),
			sz
		);
	}

	template <
		typename Sequence,
		typename F,
		typename std::enable_if_t<
			std::is_invocable<
				fold1_left_t,
				Sequence&&,
				F&&,
				decltype( (*size)(std::declval<Sequence&&>()))
			>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(Sequence && s, F && f) const {
		return operator()(
			s, 
			HBRS_MPL_FWD(f), 
			(*size)(s)
		);
	}
};

constexpr fold1_left_t fold1_left{};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_FOLD1_LEFT_HPP
