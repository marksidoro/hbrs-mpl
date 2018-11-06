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

#ifndef HBRS_MPL_CORE_CALL_IMPL_HPP
#define HBRS_MPL_CORE_CALL_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/core/implementations_of.hpp>
#include <hbrs/mpl/core/call.hpp>
#include <hbrs/mpl/core/reference_wrapper.hpp>
#include <hbrs/mpl/dt/expression.hpp>

#include <boost/hana/basic_tuple.hpp>
#include <boost/hana/type.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <
	typename Operation,
	typename... Operands
>
constexpr decltype(auto)
call_t::operator()(Operation&& o, Operands&&... os) const {
	return call_impl<
		hana::tag_of_t<Operation>,
		hana::basic_tuple<hana::tag_of_t<Operands>...>
	>::apply( HBRS_MPL_FWD(o), HBRS_MPL_FWD(os)... );
}

template <typename OperationTag, typename... OperandsTag, bool Condition>
struct call_impl<OperationTag, hana::basic_tuple<OperandsTag...>, hana::when<Condition>> : hana::default_ {
	template <
		typename Operation, 
		typename... Operands
	>
	static constexpr decltype(auto)
	apply(Operation&& o, Operands&&... os) {
		return make_expression(HBRS_MPL_FWD(o), wrap_references(HBRS_MPL_FWD(os)...));
	}
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_CALL_IMPL_HPP
