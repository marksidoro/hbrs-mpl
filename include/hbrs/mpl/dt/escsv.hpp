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

#pragma once

#ifndef HBRS_MPL_DT_ESCSV_HPP
#define HBRS_MPL_DT_ESCSV_HPP
/* equal-sized concatenated-sequences view */

#include <hbrs/mpl/fwd/dt/escsv.hpp>
#include <hbrs/mpl/core/evaluate.hpp>
#include <hbrs/mpl/detail/is_applicable.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <hbrs/mpl/detail/are_same.hpp>
#include <hbrs/mpl/fn/modulo.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <hbrs/mpl/fn/apply_at.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <boost/hana/all_of.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/assert.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <typename... Sequences>
struct escsv {
public:
	
	template<
		typename Sequence0,
		typename... SequenceN,
		typename std::enable_if_t<
			detail::is_braces_constructible<hana::tuple<Sequences...>, Sequence0&&, SequenceN&&...>::value &&
			!std::is_base_of<escsv,std::remove_reference_t<Sequence0>>::value
		>* = nullptr
	>
	constexpr
	escsv(Sequence0 && s0, SequenceN&&... sn) noexcept
	: s_{ HBRS_MPL_FWD(s0), HBRS_MPL_FWD(sn)...}
	{
		//TODO: Implement (constexpr) assert:
		/*
		using hbrs::mpl::size;
		constexpr_assert(hana::all_of(
				hana::make_tuple((*size)(HBRS_MPL_FWD(sn))...), 
				hana::equal.to((*size)(HBRS_MPL_FWD(s0)))
		));
		*/
	}
	
	constexpr
	escsv(escsv const&) noexcept = default;
	constexpr
	escsv(escsv &&) noexcept = default;
	
	constexpr escsv&
	operator=(escsv const&) noexcept = delete;
	constexpr escsv&
	operator=(escsv &&) noexcept = delete;
	
	constexpr auto 
	length() const noexcept {
		using hbrs::mpl::size;
		return (*multiply)(size(s_), size(at(s_, hana::size_c<0>)));
	}
	
	template<
		typename Index,
		typename std::enable_if_t<
			/* at(tuple<Sequences...>, integral) is only valid if all elements of all tuples are same-typed */
			!std::is_integral<Index>::value ||
			detail::is_applicable_at<
				decltype(
					hana::demux(evaluate)( hana::reverse_partial(at,std::declval<Index>()) )
				),
				decltype(
					std::declval<hana::tuple<Sequences...>>()
				)
			>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator[](Index && i) noexcept {
		//assert(0 <= i && i < size());
		return at_(s_, HBRS_MPL_FWD(i));
	}
	
	template<
		typename Index,
		typename std::enable_if_t<
			/* at(tuple<Sequences...>, integral) is only valid if all elements of all tuples are same-typed */
			!std::is_integral<Index>::value ||
			detail::is_applicable_at<
				decltype(
					hana::demux(evaluate)( hana::reverse_partial(at,std::declval<Index>()) )
				),
				decltype(
					std::declval<hana::tuple<Sequences...>>()
				)
			>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator[](Index && i) const noexcept {
		//assert(0 <= i && i < size());
		return at_(s_, HBRS_MPL_FWD(i));
	}
	
private:
	template<typename Sequences_, typename Index>
	static constexpr decltype(auto)
	at_(Sequences_ && s, Index && i) {
		using hbrs::mpl::size;
		using hbrs::mpl::apply_at;
		auto element_offset = (*modulo)(i, size(at(s, hana::size_c<0>)));
		auto tuple_offset = (*divide)(i, size(at(s, hana::size_c<0>)));
		return (*apply_at)(
			hana::demux(evaluate)( hana::reverse_partial(at,element_offset) ),
			HBRS_MPL_FWD(s),
			tuple_offset
		);
	}
	
	hana::tuple<Sequences...> s_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename... Sequences>
struct tag_of< hbrs::mpl::escsv<Sequences...> > {
	using type = hbrs::mpl::escsv_tag;
};

template <>
struct make_impl<hbrs::mpl::escsv_tag> {
	template <typename... Sequences>
	static constexpr hbrs::mpl::escsv<Sequences...>
	apply(Sequences&&... s) {
		return {HBRS_MPL_FWD(s)...};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_ESCSV_HPP
