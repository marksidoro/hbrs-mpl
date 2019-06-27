/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DETAIL_FUNCTION_OBJECT_FWD_HPP
#define HBRS_MPL_DETAIL_FUNCTION_OBJECT_FWD_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/if.hpp>
#include <hbrs/mpl/detail/is_tag_and_is_invokable.hpp>
#include <hbrs/mpl/detail/has_operator.hpp>
#include <type_traits>

#define HBRS_MPL_DECLARE_FUNCTION_OBJECT_FOR_TAG_TRY_METHOD(impl_t, tag_t, method)                                     \
	struct impl_t {                                                                                                    \
		struct invoke_test {                                                                                           \
			template <typename O, typename... Args, typename = decltype(                                               \
				std::declval<O&&>().method(std::declval<Args&&>()...)                                                  \
			)>                                                                                                         \
			constexpr void                                                                                             \
			operator()(O&& o, Args&&... args) const {}                                                                 \
		};                                                                                                             \
		                                                                                                               \
		template <                                                                                                     \
			typename O,                                                                                                \
			typename... Args,                                                                                          \
			typename std::enable_if_t<                                                                                 \
				hbrs::mpl::detail::is_tag_and_is_invokable_trait<invoke_test, O, tag_t, Args...>::value                \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(O && o, Args && ... args) const;                                                                    \
	};

#define HBRS_MPL_DEC_FO_TRY_METHOD(...)                                                                                \
	HBRS_MPL_DECLARE_FUNCTION_OBJECT_FOR_TAG_TRY_METHOD(__VA_ARGS__)

/* defines an function object of type 'impl_t' that calls a method 'method' on an object of tag 'tag_t' if that method 
 * is callable for given arguments.
 */
#define HBRS_MPL_DEFINE_FUNCTION_OBJECT_FOR_TAG_TRY_METHOD(impl_t, tag_t, method)                                      \
	template <                                                                                                         \
		typename O,                                                                                                    \
		typename... Args,                                                                                              \
		typename std::enable_if_t<                                                                                     \
			hbrs::mpl::detail::is_tag_and_is_invokable_trait<impl_t::invoke_test, O, tag_t, Args...>::value            \
		>*                                                                                                             \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	impl_t::operator()(O && o, Args && ... args) const {                                                               \
		return HBRS_MPL_FWD(o).method(HBRS_MPL_FWD(args)...);                                                          \
	}


#define HBRS_MPL_DEF_FO_TRY_METHOD(...)                                                                                \
	HBRS_MPL_DEFINE_FUNCTION_OBJECT_FOR_TAG_TRY_METHOD(__VA_ARGS__)

#define HBRS_MPL_DECLARE_FUNCTION_OBJECT_FOR_TAG_TRY_OPERATOR(impl_t, tag_t, op, op_name)                              \
	struct impl_t {                                                                                                    \
		template <                                                                                                     \
			typename LHS,                                                                                              \
			typename RHS,                                                                                              \
			typename std::enable_if_t<                                                                                 \
				boost::mpl::if_c<                                                                                      \
					std::is_same< hana::tag_of_t<LHS>, tag_t>::value &&                                                \
					std::is_same< hana::tag_of_t<RHS>, tag_t>::value,                                                  \
					std::is_invocable<has_ ## op_name ## _operator_test, LHS&&, RHS&&>,                                \
					std::false_type                                                                                    \
				>::type::value                                                                                         \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(LHS && lhs, RHS && rhs) const;                                                                      \
	};

#define HBRS_MPL_DEC_FO_TRY_OPERATOR(...)                                                                              \
	HBRS_MPL_DECLARE_FUNCTION_OBJECT_FOR_TAG_TRY_OPERATOR(__VA_ARGS__)

#define HBRS_MPL_DEFINE_FUNCTION_OBJECT_FOR_TAG_TRY_OPERATOR(impl_t, tag_t, op, op_name)                               \
	template <                                                                                                         \
		typename LHS,                                                                                                  \
		typename RHS,                                                                                                  \
		typename std::enable_if_t<                                                                                     \
			boost::mpl::if_c<                                                                                          \
				std::is_same< hana::tag_of_t<LHS>, tag_t>::value &&                                                    \
				std::is_same< hana::tag_of_t<RHS>, tag_t>::value,                                                      \
				std::is_invocable<has_ ## op_name ## _operator_test, LHS&&, RHS&&>,                                    \
				std::false_type                                                                                        \
			>::type::value                                                                                             \
		>*                                                                                                             \
	>                                                                                                                  \
	constexpr decltype(auto)                                                                                           \
	impl_t::operator()(LHS && lhs, RHS && rhs) const {                                                                 \
		return HBRS_MPL_FWD(lhs) op HBRS_MPL_FWD(rhs);                                                                 \
	}

#define HBRS_MPL_DEF_FO_TRY_OPERATOR(...)                                                                              \
	HBRS_MPL_DEFINE_FUNCTION_OBJECT_FOR_TAG_TRY_OPERATOR(__VA_ARGS__)

#endif // !HBRS_MPL_DETAIL_FUNCTION_OBJECT_FWD_HPP
