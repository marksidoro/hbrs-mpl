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

#ifndef HBRS_MPL_CORE_CALL_FWD_HPP
#define HBRS_MPL_CORE_CALL_FWD_HPP

#include <hbrs/mpl/config.hpp>

#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <
	typename OperationTag,
	typename OperandsTag,
	typename = void
>
struct call_impl : call_impl<OperationTag, OperandsTag, hana::when<true>> { };

struct call_t {
	template <
		typename Operation,
		typename... Operands
	>
	constexpr decltype(auto)
	operator()(Operation&&, Operands&&...) const;
};

constexpr call_t call{};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_CALL_FWD_HPP
