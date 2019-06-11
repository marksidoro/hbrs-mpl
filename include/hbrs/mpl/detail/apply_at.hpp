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

#ifndef HBRS_MPL_DETAIL_APPLY_AT_HPP
#define HBRS_MPL_DETAIL_APPLY_AT_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/is_homogenous_container.hpp>
#include <hbrs/mpl/detail/is_core_applicable.hpp>
#include <boost/hana/concept/sequence.hpp>
#include <boost/hana/concept/integral_constant.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/length.hpp>
#include <boost/hana/functional/apply.hpp>


#include <boost/hana/all_of.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/range.hpp>

#include <boost/hana/type.hpp>
#include <boost/hana/equal.hpp>

#include <boost/hana/at.hpp>
#include <type_traits>
#include <array>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct is_applicable_at_impl {
	template <
		typename F,
		typename S,
		typename std::enable_if_t<
			hana::Sequence<hana::tag_of_t<S>>::value &&
			//TODO: Is test for "hana::length is applicable to given sequence S" correct?
			is_core_applicable_trait<hana::length_impl<hana::tag_of_t<S>>, S&& >::value
		>* = nullptr
	>
	constexpr auto
	operator()(F && f, S && s) const {
		//Test, whether return type of f applied to all indices is equal. This is a prerequisite for using function pointers.
		return hana::all_of(
			hana::transform(
				hana::to_tuple(hana::make_range(hana::size_c<0>, hana::length(s))),
				hana::demux(hana::make_type)(
					hana::demux(hana::partial(hana::apply, f))
						(hana::partial(hana::at, s))
				)
			),
			hana::equal.to(hana::make_type(f(hana::at(s, hana::size_c<0>))))
		);
	}
	
	template <
		typename F,
		typename S,
		typename std::enable_if_t<
			!hana::Sequence<hana::tag_of_t<S>>::value ||
			!is_core_applicable_trait< hana::length_impl<hana::tag_of_t<S>>, S&&>::value
		>* = nullptr
	>
	constexpr hana::false_
	operator()(F && f, S && s) const { 
		return {};
	}
};

template<typename F, typename S>
struct is_applicable_at {
	using type = decltype(is_applicable_at_impl{}(std::declval<F>(), std::declval<S>()));
	static constexpr bool value = type::value;
};

/* apply_at for sequences whose length is known at compile time (ctl) and Where is an (possible runtime) integral. */
template<typename F, typename S, std::size_t Length>
struct apply_at_helper_t {
	constexpr 
	apply_at_helper_t()
	: fs_{ init_fs(std::make_index_sequence<Length>{}) }
	{};
	
	constexpr decltype(auto)
	operator()(F&& f, S&& s, std::size_t w) const {
		return fs_[w](HBRS_MPL_FWD(f), HBRS_MPL_FWD(s));
	}
	
private:
	template<std::size_t I>
	static constexpr decltype(auto)
	apply_at_(F&& f, S && s) {
		return hana::apply(
			HBRS_MPL_FWD(f),
			hana::at(HBRS_MPL_FWD(s), hana::size_c<I>)
		);
	}
	
	typedef decltype(&apply_at_<0>) apply_at_f;
	
	template<std::size_t... Is>
	constexpr auto
	init_fs(std::index_sequence<Is...>) const {
		return std::array<apply_at_f, Length>{{ &apply_at_<Is>... }};
	}
	
	std::array<apply_at_f, Length> fs_;
};

template<typename F, typename S, std::size_t Length>
constexpr apply_at_helper_t<F, S, Length> apply_at_helper{};

struct apply_at_impl {
	template <
		typename F,
		typename S,
		typename std::enable_if_t<
			is_applicable_at<F&&, S&&>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(F && f, S && s, std::size_t w) const {
		return apply_at_helper<F&&, S&&, decltype(hana::length(s))::value>
			(HBRS_MPL_FWD(f), HBRS_MPL_FWD(s), w);
	}
};

constexpr apply_at_impl apply_at{};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_APPLY_AT_HPP
