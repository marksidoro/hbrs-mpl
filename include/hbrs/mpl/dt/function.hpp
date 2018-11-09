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

#ifndef HBRS_MPL_DT_FUNCTION_HPP
#define HBRS_MPL_DT_FUNCTION_HPP

#include <hbrs/mpl/fwd/dt/function.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/core/call.hpp>

#define HBRS_MPL_DEFINE_FUNCTION_ARITY1(f_name, f_type)                                                                \
	template <typename T1>                                                                                             \
	constexpr decltype(auto)                                                                                           \
	f_type::operator()(T1&& t1) const {                                                                                \
		using namespace hbrs::mpl;                                                                                     \
		return call(f_name, HBRS_MPL_FWD(t1));                                                                         \
	}                                                                                                                  \
	                                                                                                                   \
	constexpr decltype(auto)                                                                                           \
	f_type::operator*() const {                                                                                        \
		return f_name;                                                          \
	}

#define HBRS_MPL_DEF_F1(...)                                                                                           \
	HBRS_MPL_DEFINE_FUNCTION_ARITY1(__VA_ARGS__)

#define HBRS_MPL_DEFINE_FUNCTION_ARITY2(f_name, f_type)                                                                \
	template <typename T1, typename T2>                                                                                \
	constexpr decltype(auto)                                                                                           \
	f_type::operator()(T1&& t1, T2 && t2) const {                                                                      \
		using namespace hbrs::mpl;                                                                                     \
		return call(f_name, HBRS_MPL_FWD(t1), HBRS_MPL_FWD(t2));                                                       \
	}                                                                                                                  \
	                                                                                                                   \
	constexpr decltype(auto)                                                                                           \
	f_type::operator*() const {                                                                                        \
		return f_name;                                                          \
	}

#define HBRS_MPL_DEF_F2(...)                                                                                           \
	HBRS_MPL_DEFINE_FUNCTION_ARITY2(__VA_ARGS__)

#define HBRS_MPL_DEFINE_FUNCTION_ARITY3(f_name, f_type)                                                                \
	template <typename T1, typename T2, typename T3>                                                                   \
	constexpr decltype(auto)                                                                                           \
	f_type::operator()(T1&& t1, T2 && t2, T3 && t3) const {                                                            \
		using namespace hbrs::mpl;                                                                                     \
		return call(f_name, HBRS_MPL_FWD(t1), HBRS_MPL_FWD(t2), HBRS_MPL_FWD(t3));                                     \
	}                                                                                                                  \
	                                                                                                                   \
	constexpr decltype(auto)                                                                                           \
	f_type::operator*() const {                                                                                        \
		return f_name;                                                          \
	}

#define HBRS_MPL_DEF_F3(...)                                                                                           \
	HBRS_MPL_DEFINE_FUNCTION_ARITY3(__VA_ARGS__)

#define HBRS_MPL_DEFINE_FUNCTION_ARITY1N(f_name, f_type)                                                               \
	template <typename T, typename... TN>                                                                              \
	constexpr decltype(auto)                                                                                           \
	f_type::operator()(T&& t, TN&&... tn) const {                                                                      \
		using namespace hbrs::mpl;                                                                                     \
		return call(f_name, HBRS_MPL_FWD(t), HBRS_MPL_FWD(tn)...);                                                     \
	}                                                                                                                  \
	                                                                                                                   \
	constexpr decltype(auto)                                                                                           \
	f_type::operator*() const {                                                                                        \
		return f_name;                                                          \
	}

#define HBRS_MPL_DEF_F1N(...)                                                                                          \
	HBRS_MPL_DEFINE_FUNCTION_ARITY1N(__VA_ARGS__)

#define HBRS_MPL_DEF_MIMPL(...)                                                                                        \
	HBRS_DEFINE_METHOD_IMPL(__VA_ARGS__)

#endif // !HBRS_MPL_DT_FUNCTION_HPP
