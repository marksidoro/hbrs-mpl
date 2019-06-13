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

#ifndef HBRS_MPL_DT_FUNCTION_FWD_HPP
#define HBRS_MPL_DT_FUNCTION_FWD_HPP

#include <hbrs/mpl/config.hpp>

#define HBRS_MPL_DECLARE_FUNCTION_ARITY1(f_name, f_type)                                                               \
	struct f_type {                                                                                                    \
		template <typename T>                                                                                          \
		constexpr decltype(auto)                                                                                       \
		operator()(T&&) const;                                                                                         \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		operator*() const;                                                                                             \
	};                                                                                                                 \
	                                                                                                                   \
	constexpr f_type f_name{};

#define HBRS_MPL_DEC_F1(...)                                                                                           \
	HBRS_MPL_DECLARE_FUNCTION_ARITY1(__VA_ARGS__)
	
#define HBRS_MPL_DECLARE_FUNCTION_ARITY2(f_name, f_type)                                                               \
	struct f_type {                                                                                                    \
		template <typename T1, typename T2>                                                                            \
		constexpr decltype(auto)                                                                                       \
		operator()(T1&&, T2&&) const;                                                                                  \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		operator*() const;                                                                                             \
	};                                                                                                                 \
	                                                                                                                   \
	constexpr f_type f_name{};

#define HBRS_MPL_DEC_F2(...)                                                                                           \
	HBRS_MPL_DECLARE_FUNCTION_ARITY2(__VA_ARGS__)

#define HBRS_MPL_DECLARE_FUNCTION_ARITY3(f_name, f_type)                                                               \
	struct f_type {                                                                                                    \
		template <typename T1, typename T2, typename T3>                                                               \
		constexpr decltype(auto)                                                                                       \
		operator()(T1&&, T2&&, T3&&) const;                                                                            \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		operator*() const;                                                                                             \
	};                                                                                                                 \
	                                                                                                                   \
	constexpr f_type f_name{};

#define HBRS_MPL_DEC_F3(...)                                                                                           \
	HBRS_MPL_DECLARE_FUNCTION_ARITY3(__VA_ARGS__)

#define HBRS_MPL_DECLARE_FUNCTION_ARITY1N(f_name, f_type)                                                              \
	struct f_type {                                                                                                    \
		template <typename T, typename... TN>                                                                          \
		constexpr decltype(auto)                                                                                       \
		operator()(T&&, TN&&...) const;                                                                                \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		operator*() const;                                                                                             \
	};                                                                                                                 \
	                                                                                                                   \
	constexpr f_type f_name{};

#define HBRS_MPL_DEC_F1N(...)                                                                                          \
	HBRS_MPL_DECLARE_FUNCTION_ARITY1N(__VA_ARGS__)

#endif // !HBRS_MPL_DT_FUNCTION_FWD_HPP
