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

#ifndef HBRS_MPL_DT_USCSV_IMPL_HPP
#define HBRS_MPL_DT_USCSV_IMPL_HPP
/* unequal-sized concatenated-sequences view */

#include <hbrs/mpl/dt/uscsv/fwd.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/apply_at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <boost/hana/functional/id.hpp>
#include <boost/hana/functional/reverse_partial.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/sum.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/less.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/slice.hpp>
#include <boost/hana/count_if.hpp>
#include <array>


HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename T,
	typename std::enable_if_t< hana::IntegralConstant<hana::tag_of_t<T>>::value >* = nullptr
>
constexpr auto
is_unsigned_constant_(T, int) { return hana::trait<std::is_unsigned>(hana::make_type( hana::value<T>() )) ; }

template<typename T>
constexpr hana::false_
is_unsigned_constant_(T, ...) { return {}; }

template<typename T>
struct is_unsigned_constant : decltype(is_unsigned_constant_(std::declval<T>(), 0)) {};

template<typename T>
struct is_unsigned : hana::bool_<
	(std::is_integral<T>::value && std::is_unsigned<T>::value) || is_unsigned_constant<T>::value
> {};

/* namespace detail */ }

template <typename... Sequences>
struct uscsv {
	
	template<
		typename Sequence0,
		typename... SequenceN,
		typename std::enable_if_t<
			detail::is_braces_constructible<hana::tuple<Sequences...>, Sequence0&&, SequenceN&&...>::value &&
			!std::is_base_of<uscsv,std::remove_reference_t<Sequence0>>::value
		>* = nullptr
	>
	constexpr
	uscsv(Sequence0 && s0, SequenceN&&... sn) noexcept
	: s_{HBRS_MPL_FWD(s0), HBRS_MPL_FWD(sn)...},
	o_ {offsets(std::index_sequence_for<Sequence0, SequenceN...>{},  hana::make_tuple(HBRS_MPL_FWD(s0), HBRS_MPL_FWD(sn)...))}
	{}
	
	constexpr
	uscsv(uscsv const&) noexcept = default;
	constexpr
	uscsv(uscsv &&) noexcept = default;
	
	constexpr uscsv&
	operator=(uscsv const&) noexcept = delete;
	constexpr uscsv&
	operator=(uscsv &&) noexcept = delete;
	
	constexpr auto
	length() const noexcept {
		return total_length(s_);
	}
	
	template<
		typename Index,
		typename std::enable_if_t<
			hana::trait<detail::is_unsigned>(hana::type_c<Index>)
		>* = nullptr
	>
	constexpr decltype(auto)
	operator[](Index && i) noexcept {
		//assert(0 <= i && i < length());
		//TODO: count_if and less.than correct?
		return at_(s_, o_, i, hana::count_if(o_, hana::less.than(i)));
	}
	
	template<
		typename Index,
		typename std::enable_if_t<
			hana::trait<detail::is_unsigned>(hana::type_c<Index>)
		>* = nullptr
	>
	constexpr decltype(auto)
	operator[](Index && i) const noexcept {
		//assert(0 <= i && i < length());
		//TODO: count_if and less.than correct?
		return at_(s_, o_, i, hana::count_if(o_, hana::less.than(i)));
	}
	
private:
	template<typename Sequence>
	static constexpr auto
	total_length(Sequence && s) {
		using hbrs::mpl::size;
		return (hana::length(s) == hana::size_c<0>)
			? hana::size_c<0>
			: hana::sum<std::size_t>(
				hana::transform(HBRS_MPL_FWD(s), *size)
			);
	}
	
	template<typename Sequences_, std::size_t... Is>
	static constexpr auto
	offsets(std::index_sequence<Is...>, Sequences_ && s) {
		return std::array<std::size_t, sizeof...(Is)> {{
			(
				total_length(
					hana::slice(HBRS_MPL_FWD(s), hana::make_range(hana::size_c<0>, hana::size_c<Is+1>))
				) - hana::size_c<1>
			)...
		}};
	}
	
	template<typename Sequences_, typename Offsets, typename Index, typename Offset>
	static constexpr decltype(auto)
	at_(Sequences_ && s, Offsets && os, Index && i, Offset && o) {
		return (*apply_at)(
			hana::reverse_partial(*at,
				(o==hana::size_c<0>) 
					? i
					: hana::minus(
						HBRS_MPL_FWD(i),
						HBRS_MPL_FWD(os)[o - hana::size_c<1>] + hana::size_c<1>
					)
			),
			HBRS_MPL_FWD(s),
			HBRS_MPL_FWD(o)
		);
	}
	
	hana::tuple<Sequences...> s_;
	std::array<std::size_t, sizeof...(Sequences)> const o_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename... Sequences>
struct tag_of< hbrs::mpl::uscsv<Sequences...> > {
	using type = hbrs::mpl::uscsv_tag;
};

template <>
struct make_impl<hbrs::mpl::uscsv_tag> {
	template <typename... Sequences>
	static constexpr hbrs::mpl::uscsv<Sequences...>
	apply(Sequences&&... s) {
		return {HBRS_MPL_FWD(s)...};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_USCSV_IMPL_HPP
