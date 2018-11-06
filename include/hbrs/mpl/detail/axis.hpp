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

#pragma once

#ifndef HBRS_MPL_DETAIL_AXIS_HPP
#define HBRS_MPL_DETAIL_AXIS_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <type_traits>
#include <ostream>

/* NOTE: 
 * Before we define our new types we can only include the forward declarations <hbrs/mpl/fwd/fn/equal.hpp>... .
 * Once all types have been defined we can include the function definitions <hbrs/mpl/fn/equal.hpp>... .
 * Practically this means that your header must look like this:
 * 
 *  #include <hbrs/mpl/detail/axis.hpp>
 *  HBRS_MPL_DEFINE_AXIS_2D(range, Range, first, First, last, Last)
 *  #include <hbrs/mpl/fn/equal.hpp>
 *  #include <hbrs/mpl/fn/and.hpp>
 *  #include <hbrs/mpl/fn/not.hpp>
 *  HBRS_MPL_DEFINE_AXIS_2D_EQUALITY_OPERATORS(range, Range, first, First, last, Last)
 * 
 * Reason:
 * For example, matrix_index.hpp includes axis.hpp. If axis.hpp would include <hbrs/mpl/fn/equal.hpp>... instead of only
 * its fwd decls <hbrs/mpl/fwd/fn/equal.hpp>..., then all their implementations would be included, too. 
 * 
 * (a) They might include matrix_size.hpp which again includes axis.hpp. But as axis.hpp is not yet parsed completely 
 *     (preprocessor is still parsing its includes), the pp macros HBRS_MPL_DEFINE_AXIS_2D... are not yet defined and 
 *     thus inclusion of matrix_size.hpp fails. 
 * (b) They might include matrix_index.hpp which is currently parsed. Even if compilation might not fail due to (a) it 
 *     might silently cause unwanted behaviour if e.g. types are defined but equality operators are not.
 */

#include <hbrs/mpl/fwd/fn/equal.hpp>
#include <hbrs/mpl/fwd/fn/and.hpp>
#include <hbrs/mpl/fwd/fn/not.hpp>

#define HBRS_MPL_DEFINE_AXIS_2D(name, Name, name0, Name0, name1, Name1)                                                \
	HBRS_MPL_NAMESPACE_BEGIN                                                                                           \
	                                                                                                                   \
	template<typename Name0, typename Name1>                                                                           \
	struct name {                                                                                                      \
		                                                                                                               \
		template<typename Name0 ## _, typename Name1 ## _>                                                             \
		friend struct name;                                                                                            \
		                                                                                                               \
		template<                                                                                                      \
			typename Name0 ## _ = Name0,                                                                               \
			typename Name1 ## _ = Name1,                                                                               \
			typename std::enable_if_t<                                                                                 \
				std::is_default_constructible<Name0 ## _>::value &&                                                    \
				std::is_default_constructible<Name1 ## _>::value                                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr                                                                                                      \
		name() {}                                                                                                      \
		                                                                                                               \
		template<typename Name0 ## _, typename Name1 ## _>                                                             \
		constexpr                                                                                                      \
		name(Name0 ## _ && name0, Name1 ## _ && name1)                                                                 \
		: name0 ## _{HBRS_MPL_FWD(name0)}, name1 ## _{HBRS_MPL_FWD(name1)} {}                                          \
		                                                                                                               \
		constexpr                                                                                                      \
		name(name const&) = default;                                                                                   \
		                                                                                                               \
		template<                                                                                                      \
			typename Name0 ## _,                                                                                       \
			typename Name1 ## _,                                                                                       \
			typename std::enable_if_t<                                                                                 \
				hbrs::mpl::detail::is_braces_constructible< Name0, Name0 ## _>::value &&                               \
				hbrs::mpl::detail::is_braces_constructible< Name1, Name1 ## _>::value                                  \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr                                                                                                      \
		name(name<Name0 ## _, Name1 ## _> const& rhs)                                                                  \
		: name0 ## _{rhs.name0 ## _}, name1 ## _{rhs.name1 ## _} {}                                                    \
		                                                                                                               \
		template<                                                                                                      \
			typename Name0 ## _,                                                                                       \
			typename Name1 ## _,                                                                                       \
			typename std::enable_if_t<                                                                                 \
				hbrs::mpl::detail::is_braces_constructible< Name0, Name0 ## _>::value &&                               \
				hbrs::mpl::detail::is_braces_constructible< Name1, Name1 ## _>::value                                  \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr                                                                                                      \
		name(name<Name0 ## _, Name1 ## _> && rhs)                                                                      \
		: name0 ## _{HBRS_MPL_FWD(rhs).name0 ## _}, name1 ## _{HBRS_MPL_FWD(rhs).name1 ## _} {}                        \
		                                                                                                               \
		template<                                                                                                      \
			typename Name0 ## _,                                                                                       \
			typename Name1 ## _,                                                                                       \
			typename std::enable_if_t<                                                                                 \
				(                                                                                                      \
					!hbrs::mpl::detail::is_braces_constructible< Name0, Name0 ## _>::value ||                          \
					!hbrs::mpl::detail::is_braces_constructible< Name1, Name1 ## _>::value                             \
				) &&                                                                                                   \
				std::is_convertible<Name0 ## _, Name0>::value &&                                                       \
				std::is_convertible<Name1 ## _, Name1>::value &&                                                       \
				std::is_arithmetic<Name0 ## _>::value &&                                                               \
				std::is_arithmetic<Name1 ## _>::value &&                                                               \
				std::is_arithmetic<Name0>::value &&                                                                    \
				std::is_arithmetic<Name1>::value                                                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		name(name<Name0 ## _, Name1 ## _> const& rhs)                                                                  \
		: name0 ## _{boost::numeric_cast< Name0 >(rhs.name0 ## _)},                                                    \
		  name1 ## _{boost::numeric_cast< Name1 >(rhs.name1 ## _)} {}                                                  \
		                                                                                                               \
		constexpr                                                                                                      \
		name(name &&) = default;                                                                                       \
		                                                                                                               \
		name&                                                                                                          \
		operator=(name const&) = default;                                                                              \
		name&                                                                                                          \
		operator=(name &&) = default;                                                                                  \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		name0() & { return (name0 ## _); };                                                                            \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		name0() const& { return (name0 ## _); };                                                                       \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		name0() && { return HBRS_MPL_FWD(name0 ## _); };                                                               \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		name1() & { return (name1 ## _); };                                                                            \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		name1() const& { return (name1 ## _); };                                                                       \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		name1() && { return HBRS_MPL_FWD(name1 ## _); };                                                               \
                                                                                                                       \
	private:                                                                                                           \
		Name0 name0 ## _;                                                                                              \
		Name1 name1 ## _;                                                                                              \
	};                                                                                                                 \
	HBRS_MPL_NAMESPACE_END                                                                                             \
	                                                                                                                   \
	namespace boost { namespace hana {                                                                                 \
                                                                                                                       \
	template <typename Name0, typename Name1>                                                                          \
	struct tag_of< hbrs::mpl::name<Name0, Name1> > {                                                                   \
		using type = hbrs::mpl::name ## _tag;                                                                          \
	};                                                                                                                 \
                                                                                                                       \
	template <>                                                                                                        \
	struct make_impl<hbrs::mpl::name ## _tag> {                                                                        \
		template <typename Name0, typename Name1>                                                                      \
		static constexpr hbrs::mpl::name<                                                                              \
			std::decay_t<Name0>,                                                                                       \
			std::decay_t<Name1>                                                                                        \
		>                                                                                                              \
		apply(Name0 && name0, Name1 && name1) {                                                                        \
			return { HBRS_MPL_FWD(name0), HBRS_MPL_FWD(name1) };                                                       \
		}                                                                                                              \
	};                                                                                                                 \
	                                                                                                                   \
	/* namespace hana */ } /* namespace boost */ }

#define HBRS_MPL_DEFINE_AXIS_2D_EQUALITY_OPERATORS(name, Name, name0, Name0, name1, Name1)                             \
	HBRS_MPL_NAMESPACE_BEGIN                                                                                           \
	/* TODO: Write macro for binary operators! */                                                                      \
	template<                                                                                                          \
		typename L,                                                                                                    \
		typename R,                                                                                                    \
		typename std::enable_if_t<                                                                                     \
			std::is_same< name ## _tag, hana::tag_of_t<L> >::value &&                                                  \
			std::is_same< name ## _tag, hana::tag_of_t<R> >::value &&                                                  \
			std::is_same< hana::tag_of_t<L>, hana::tag_of_t<R> >:: value /* TODO: is_same required? */                 \
		>* = nullptr                                                                                                   \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	operator==(L && l, R && r) {                                                                                       \
		return (*and_)(                                                                                                \
			equal(HBRS_MPL_FWD(l).name0(), HBRS_MPL_FWD(r).name0()),                                                   \
			equal(HBRS_MPL_FWD(l).name1(), HBRS_MPL_FWD(r).name1())                                                    \
		);                                                                                                             \
	}                                                                                                                  \
                                                                                                                       \
	template<                                                                                                          \
		typename L,                                                                                                    \
		typename R,                                                                                                    \
		typename std::enable_if_t<                                                                                     \
			std::is_same< name ## _tag, hana::tag_of_t<L> >::value &&                                                  \
			std::is_same< name ## _tag, hana::tag_of_t<R> >::value &&                                                  \
			std::is_same< hana::tag_of_t<L>, hana::tag_of_t<R> >:: value /* TODO: is_same required? */                 \
		>* = nullptr                                                                                                   \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	operator!=(L && l, R && r) {                                                                                       \
		return (*not_)(equal(HBRS_MPL_FWD(l), HBRS_MPL_FWD(r)));                                                       \
	}                                                                                                                  \
                                                                                                                       \
	template<typename Name0, typename Name1>                                                                           \
	std::ostream& operator<<(std::ostream& os, name<Name0, Name1> const& x) {                                          \
		os << "name0:=" << x.name0() << " name1:=" << x.name1();                                                       \
		return os;                                                                                                     \
	}                                                                                                                  \
	HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_AXIS_HPP
