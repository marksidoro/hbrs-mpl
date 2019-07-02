/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_CORE_REFERENCE_WRAPPER_IMPL_HPP
#define HBRS_MPL_CORE_REFERENCE_WRAPPER_IMPL_HPP

#include "fwd.hpp"
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/functional/apply.hpp>
#include <type_traits>
#include <functional>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

/* lightweight constexpr impl of std::reference_wrapper, adapted from /usr/include/c++/6/functional */
template<typename T>
struct lvalue_reference_wrapper {
	typedef T type;
	
	constexpr
	lvalue_reference_wrapper(T& t) noexcept
	: t_{t}
	{}
	
	template<
		typename T_,
		typename std::enable_if_t<
			is_braces_constructible<T, T_&&>::value &&
			!std::is_base_of<lvalue_reference_wrapper, std::remove_reference_t<T_> >::value
		>* = nullptr
	>
	constexpr
	lvalue_reference_wrapper(T&&) = delete;
	
	constexpr
	lvalue_reference_wrapper(lvalue_reference_wrapper const&) = default;

	constexpr lvalue_reference_wrapper&
	operator=(lvalue_reference_wrapper const&) = default;

	constexpr
	operator T&() const noexcept { return this->get(); }

	constexpr T&
	get() const noexcept { return t_; }
	
	template<typename... Args>
	constexpr decltype(auto)
	operator()(Args&&... as) const {
		return hana::apply(get(), HBRS_MPL_FWD(as)...);
	}
	
private:
	T& t_;
};


template<typename T>
struct rvalue_reference_wrapper {
	
	template<
		typename T_,
		typename std::enable_if_t<
			is_braces_constructible<T, T_&&>::value &&
			!std::is_base_of< rvalue_reference_wrapper, std::remove_reference_t<T_&&> >::value
		>* = nullptr
	>
	constexpr
	rvalue_reference_wrapper(T_ && t) noexcept
	: t_{HBRS_MPL_FWD(t)} {} 
	
	constexpr
	rvalue_reference_wrapper(rvalue_reference_wrapper const&) = default;
	
	constexpr
	rvalue_reference_wrapper(rvalue_reference_wrapper &&) = default;
	
	constexpr rvalue_reference_wrapper&
	operator=(rvalue_reference_wrapper const&) = delete;
	
	constexpr rvalue_reference_wrapper&
	operator=(rvalue_reference_wrapper &&) = default;
	
	constexpr
	operator T &() & noexcept { return t_; }
	
	constexpr
	operator T const&() const& noexcept { return t_; }
	
	constexpr
	operator T &&() && noexcept { return HBRS_MPL_FWD(t_); }
	
	constexpr T &
	get() & noexcept { return t_; }
	
	constexpr T const&
	get() const& noexcept { return t_; }
	
	constexpr T &&
	get() && noexcept { return HBRS_MPL_FWD(t_); }
	
private:
	T t_;
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END



namespace std {

template<typename T> 
struct is_bind_expression< 
	hbrs::mpl::detail::rvalue_reference_wrapper<T>
> : std::true_type {}; 

/* namespace std */ }



namespace boost { namespace hana {

template <typename T>
struct tag_of< hbrs::mpl::detail::lvalue_reference_wrapper<T> > {
	using type = hbrs::mpl::detail::lvalue_reference_wrapper_tag;
};


template <typename T>
struct tag_of< hbrs::mpl::detail::rvalue_reference_wrapper<T> > {
	using type = hbrs::mpl::detail::rvalue_reference_wrapper_tag;
};

template <>
struct make_impl<hbrs::mpl::detail::lvalue_reference_wrapper_tag> {
	
	template <
		typename T,
		typename std::enable_if_t< 
			std::is_lvalue_reference<T&&>::value
		>* = nullptr
	>
	static constexpr hbrs::mpl::detail::lvalue_reference_wrapper< std::remove_reference_t<T&&> >
	apply(T && t) {
		return {HBRS_MPL_FWD(t)};
	}
	
};

template <>
struct make_impl<hbrs::mpl::detail::rvalue_reference_wrapper_tag> {
	
	template <
		typename T,
		typename std::enable_if_t< std::is_rvalue_reference<T&&>::value >* = nullptr
	>
	static constexpr hbrs::mpl::detail::rvalue_reference_wrapper< std::remove_reference_t<T&&> >
	apply(T && t) {
		return {HBRS_MPL_FWD(t)};
	}
};

/* namespace hana */ } /* namespace boost */ }

#include <boost/hana/concept/sequence.hpp>
#include <hbrs/mpl/detail/transform_and_forward.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/transform.hpp>
#include <boost/hana/basic_tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

HBRS_MPL_DEF_CF1(unwrap_reference)

template <typename ValueTag, bool Condition>
struct unwrap_reference_impl<ValueTag, hana::when<Condition>> : hana::default_ {
	template <
		typename Value,
		typename std::enable_if_t< std::is_rvalue_reference<Value&&>::value >* = nullptr
	>
	static constexpr decltype(auto)
	/* return type must be decltype(auto) and not auto because unwrap_references (plural!) 
	 * must preserve rvalue references e.g. for use in invoke_evaluate! 
	 */
	apply(Value && v) {
		return HBRS_MPL_FWD(v);
	}
	
	template <
		typename Value,
		typename std::enable_if_t< std::is_lvalue_reference<Value&&>::value >* = nullptr
	>
	static constexpr decltype(auto)
	apply(Value && v) {
		return HBRS_MPL_FWD(v);
	}
	
	template <
		typename Value,
		typename std::enable_if_t< !std::is_reference<Value&&>::value >* = nullptr
	>
	static constexpr auto
	apply(Value && v) = delete; //IS THIS ACTUALLY CALLABLE?!?
};

template <>
struct unwrap_reference_impl<detail::lvalue_reference_wrapper_tag> {
	template <typename T>
	static constexpr decltype(auto)
	apply(T && t) {
		return HBRS_MPL_FWD(t).get();
	}
};

template <>
struct unwrap_reference_impl<detail::rvalue_reference_wrapper_tag> {
	template <typename T>
	static constexpr decltype(auto)
	apply(T && t) {
		return HBRS_MPL_FWD(t).get();
	}
};

HBRS_MPL_DEF_CF1(unwrap_references)

template <typename STag>
struct unwrap_references_impl<STag, hana::when<hana::Sequence<STag>::value>> {
	template <typename S>
	static constexpr decltype(auto)
	apply(S&& s) {
		return detail::transform_and_forward(HBRS_MPL_FWD(s), unwrap_reference);
	}
};



HBRS_MPL_DEF_CF1(wrap_reference)

template <typename Tag, bool Condition>
struct wrap_reference_impl<Tag, hana::when<Condition>> : hana::default_ {
	template <
		typename T,
		typename std::enable_if_t< 
			std::is_lvalue_reference<T&&>::value
		>* = nullptr
	>
	static constexpr auto
	apply(T && t) {
		return detail::make_lvalue_reference_wrapper(HBRS_MPL_FWD(t));
	}

	template <
		typename T,
		typename std::enable_if_t<
			std::is_rvalue_reference<T&&>::value
		>* = nullptr
	>
	static constexpr auto
	apply(T && t) {
		return detail::make_rvalue_reference_wrapper(HBRS_MPL_FWD(t));
	}
	
	template <
		typename T,
		typename std::enable_if_t< 
			!std::is_reference<T&&>::value
		>* = nullptr
	>
	static constexpr auto
	apply(T && t) = delete; //IS THIS ACTUALLY CALLABLE?!?
	
};

template <>
struct wrap_reference_impl<detail::lvalue_reference_wrapper_tag> {
	template <typename T>
	static constexpr auto
	apply(T && t) {
		return HBRS_MPL_FWD(t);
	}
};

template <>
struct wrap_reference_impl<detail::rvalue_reference_wrapper_tag> {
	template <typename T>
	static constexpr auto
	apply(T && t) {
		return HBRS_MPL_FWD(t);
	}
};

HBRS_MPL_DEF_CFV(wrap_references)

template <typename... Tags, bool Condition>
struct wrap_references_impl<hana::basic_tuple<Tags...>, hana::when<Condition>> : hana::default_ {
	template <typename... Ts>
	static constexpr decltype(auto)
	apply(Ts&& ...ts) {
		return 
		/* When using detail::transform_and_forward then rvalues will be stored as tuple<T&&>. 
		 * this will e.g. prevent expressions to be copyable */
			hana::transform(
				detail::forward_as_tuple(HBRS_MPL_FWD(ts)...),
				wrap_reference
			);
	}
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_REFERENCE_WRAPPER_IMPL_HPP
