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

#ifndef HBRS_MPL_DT_EVALUATION_CONTEXT_IMPL_HPP
#define HBRS_MPL_DT_EVALUATION_CONTEXT_IMPL_HPP

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template<typename Operation, typename OperationImpls, typename Operands, typename ChoosenImpl, typename Context>
struct evaluation_context{
	template<
		typename Operation_, 
		typename OperationImpls_,
		typename Operands_, 
		typename ChoosenImpl_, 
		typename Context_
	>
	constexpr 
	evaluation_context(Operation_ && o, OperationImpls_ && ois, Operands_ && os, ChoosenImpl_ && ci, Context_ && c) 
	: 
		operation_{HBRS_MPL_FWD(o)},
		operation_impls_{HBRS_MPL_FWD(ois)},
		operands_{HBRS_MPL_FWD(os)},
		choosen_impl_{HBRS_MPL_FWD(ci)},
		context_{HBRS_MPL_FWD(c)}
	{}
	
	constexpr 
	evaluation_context() : operation_{}, operation_impls_{}, operands_{}, choosen_impl_{}, context_{} {}
	
	constexpr 
	evaluation_context(evaluation_context const&) = default;
	
	constexpr 
	evaluation_context(evaluation_context &&) = default;
	
	constexpr evaluation_context&
	operator=(evaluation_context const&) = default;
	
	constexpr evaluation_context&
	operator=(evaluation_context &&) = default;
	
	constexpr decltype(auto)
	operation() & { return (operation_); }
	
	constexpr decltype(auto)
	operation() const& { return (operation_); }
	
	constexpr decltype(auto)
	operation() && { return HBRS_MPL_FWD(operation_); }
	
	constexpr decltype(auto)
	operation_impls() & { return (operation_impls_); }
	
	constexpr decltype(auto)
	operation_impls() const& { return (operation_impls_); }
	
	constexpr decltype(auto)
	operation_impls() && { return HBRS_MPL_FWD(operation_impls_); }
	
	constexpr decltype(auto)
	operands() & { return (operands_); }
	
	constexpr decltype(auto)
	operands() const& { return (operands_); }
	
	constexpr decltype(auto)
	operands() && { return HBRS_MPL_FWD(operands_); }
	
	constexpr decltype(auto)
	choosen_impl() & { return (choosen_impl_); }
	
	constexpr decltype(auto)
	choosen_impl() const& { return (choosen_impl_); }
	
	constexpr decltype(auto)
	choosen_impl() && { return HBRS_MPL_FWD(choosen_impl_); }
	
	constexpr decltype(auto)
	context() & { return (context_); }
	
	constexpr decltype(auto)
	context() const& { return (context_); }
	
	constexpr decltype(auto)
	context() && { return HBRS_MPL_FWD(context_); }
	
private:
	Operation operation_;
	OperationImpls operation_impls_;
	Operands operands_;
	ChoosenImpl choosen_impl_;
	Context context_;
};

HBRS_MPL_NAMESPACE_END


namespace boost { namespace hana {

template <typename Operation, typename OperationImpls, typename Operands, typename ChoosenImpl, typename Context>
struct tag_of< hbrs::mpl::evaluation_context<Operation, OperationImpls, Operands, ChoosenImpl, Context> > {
	using type = hbrs::mpl::evaluation_context_tag;
};

template <>
struct make_impl<hbrs::mpl::evaluation_context_tag> {
	template <typename Operation, typename OperationImpls, typename Operands, typename ChoosenImpl, typename Context>
	static constexpr hbrs::mpl::evaluation_context<
		std::decay_t<Operation>, 
		std::decay_t<OperationImpls>, 
		std::decay_t<Operands>, 
		std::decay_t<ChoosenImpl>, 
		std::decay_t<Context>
	>
	apply(Operation && o, OperationImpls && ois, Operands && os, ChoosenImpl && ci, Context && c) {
		return {
			HBRS_MPL_FWD(o),
			HBRS_MPL_FWD(ois),
			HBRS_MPL_FWD(os),
			HBRS_MPL_FWD(ci), 
			HBRS_MPL_FWD(c)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_EVALUATION_CONTEXT_IMPL_HPP
