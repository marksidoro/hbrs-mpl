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

#ifndef HBRS_MPL_DT_EXPRESSION_HPP
#define HBRS_MPL_DT_EXPRESSION_HPP

#include <hbrs/mpl/dt/expression/fwd.hpp>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>

#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <
	typename Operation,
	typename Operands
>
struct expression {
public:
	
	template<typename Operation_, typename Operands_>
	constexpr
	expression(Operation_ && o, Operands_ && os) 
	:
	operation_{HBRS_MPL_FWD(o)},
	operands_{HBRS_MPL_FWD(os)} {}
	
	constexpr
	expression(expression const&) = default;
	
	constexpr
	expression(expression &&) = default;
	
	constexpr expression&
	operator=(expression const&) = default;
	
	constexpr expression&
	operator=(expression &&) = delete;
	
	constexpr decltype(auto)
	operation() & { return (operation_); }
	
	constexpr decltype(auto)
	operation() const& { return (operation_); }
	
	constexpr decltype(auto)
	operation() && { return HBRS_MPL_FWD(operation_); }
	
	constexpr decltype(auto)
	operands() & { return (operands_); }
	
	constexpr decltype(auto)
	operands() const& { return (operands_); }
	
	constexpr decltype(auto)
	operands() && { return HBRS_MPL_FWD(operands_); }
private:
	Operation operation_;
	Operands operands_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <
	typename Operation,
	typename Operands
>
struct tag_of< hbrs::mpl::expression<Operation,Operands> > {
	using type = hbrs::mpl::expression_tag;
};

template <>
struct make_impl<hbrs::mpl::expression_tag> {
	template <
		typename Operation,
		typename Operands
	>
	static constexpr hbrs::mpl::expression<
		std::decay_t<Operation>,
		std::decay_t<Operands>
	>
	apply(Operation && o, Operands && os) {
		return {
			HBRS_MPL_FWD(o),
			HBRS_MPL_FWD(os)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_EXPRESSION_HPP
