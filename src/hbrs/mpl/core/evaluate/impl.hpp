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

#ifndef HBRS_MPL_CORE_EVALUATE_IMPL_HPP
#define HBRS_MPL_CORE_EVALUATE_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/core/implementations_of.hpp>
#include <hbrs/mpl/core/is_error.hpp>
#include <hbrs/mpl/core/is_error_cause.hpp>
#include <hbrs/mpl/core/annotation.hpp>
#include <hbrs/mpl/core/reference_wrapper.hpp>

#include <hbrs/mpl/detail/forward_and_compose.hpp>
#include <hbrs/mpl/detail/type_of.hpp>

#include <hbrs/mpl/dt/evaluation_context.hpp>
#include <hbrs/mpl/dt/error.hpp>
#include <hbrs/mpl/dt/stack_frame.hpp>
#include <hbrs/mpl/dt/expression.hpp>

#include <boost/hana/transform.hpp>
#include <boost/hana/unpack.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/front.hpp>
#include <boost/hana/functional/apply.hpp>
#include <boost/hana/functional/partial.hpp>
#include <boost/hana/functional/reverse_partial.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/functional/demux.hpp>
#include <boost/hana/optional.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/ext/std/tuple.hpp>

#include <tuple>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

HBRS_MPL_DEF_CF3(choose_implementation)

template <typename OperationTag, typename OperationImplsTag, typename OperandsTag, bool Condition>
struct choose_implementation_impl<OperationTag, OperationImplsTag, OperandsTag, hana::when<Condition>> : hana::default_ {
	
	template <
		typename Operation,
		typename OperationImpls,
		typename Operands
	>
	static constexpr auto
	apply(Operation && o, OperationImpls && ois, Operands && os) {
		return hana::make_optional(hana::front(HBRS_MPL_FWD(ois)));
	}
};


// HBRS_MPL_DEF_CF2(evaluate)

template <
	typename Expression,
	typename Context
>
constexpr decltype(auto)
evaluate_t::operator()(Expression&& e, Context&& c) const {
	return evaluate_impl<
		hana::tag_of_t<Expression>,
		hana::tag_of_t<Context>
	>::apply( HBRS_MPL_FWD(e), HBRS_MPL_FWD(c) );
}

namespace detail {

	HBRS_MPL_DEC_CF4(is_recursive_impl)
	HBRS_MPL_DEF_CF4(is_recursive_impl)
	
	HBRS_MPL_DEC_CF5(is_recursive_impl_helper)
	HBRS_MPL_DEF_CF5(is_recursive_impl_helper)
	
	
	template <
		typename IsRecursiveTag,
		typename OperationTag,
		typename OperandsTypesTag, 
		typename ContextTag, 
		typename ImplTag
	>
	struct is_recursive_impl_helper_impl<
		IsRecursiveTag,
		OperationTag,
		OperandsTypesTag, 
		ContextTag, 
		ImplTag
	> {
		
		template <
			typename Operation, 
			typename OperandsTypes,
			typename Context,
			typename Impl
		>
		static constexpr hana::true_
		apply(
			hana::true_,
			Operation && o, 
			OperandsTypes && os, 
			Context && c,
			Impl && i
		) {
			return hana::true_c;
		}
		
		template <
			typename Operation, 
			typename OperandsTypes,
			typename Context,
			typename Impl
		>
		static constexpr auto
		apply(
			hana::false_, 
			Operation && o, 
			OperandsTypes && os, 
			Context && c,
			Impl && i
		) {
			return is_recursive_impl(
				HBRS_MPL_FWD(o),
				HBRS_MPL_FWD(os),
				HBRS_MPL_FWD(c),
				HBRS_MPL_FWD(i)
			);
		}
	};
	
	
	template <
		typename OperationTag,
		typename OperandsTypesTag,
		typename ImplTag
	>
	struct is_recursive_impl_impl<
		OperationTag,
		OperandsTypesTag, 
		evaluation_context_tag, 
		ImplTag
	> {
		
		template <
			typename Operation, 
			typename OperandsTypes,
			typename Context,
			typename Impl
		>
		static constexpr auto
		apply(
			Operation && o, 
			OperandsTypes && os, 
			Context && c,
			Impl && i
		) {
			return is_recursive_impl_helper(
				(hana::typeid_(i) == c.choosen_impl()) &&
				(hana::typeid_(os) == hana::typeid_(c.operands())),
				HBRS_MPL_FWD(o),
				HBRS_MPL_FWD(os),
				HBRS_MPL_FWD(c).context(),
				HBRS_MPL_FWD(i)
			);
		}
	};
	
	template <
		typename OperationTag,
		typename OperandsTypesTag,
		typename ImplTag
	>
	struct is_recursive_impl_impl<
		OperationTag,
		OperandsTypesTag, 
		no_context_tag, 
		ImplTag
	> {
		
		template <
			typename Operation, 
			typename OperandsTypes,
			typename Context,
			typename Impl
		>
		static constexpr hana::false_
		apply(
			Operation &&, OperandsTypes &&, Context &&, Impl &&
		) {
			return hana::false_c;
		}
	};
	
	HBRS_MPL_DEC_CF5(invoke_evaluate)
	HBRS_MPL_DEF_CF5(invoke_evaluate)
	
	template <
		typename OperationTag, 
		typename OperationImplsTag, 
		typename OperandsTag, 
		typename ContextTag, 
		typename ChoosenImplTag, 
		bool Condition
	>
	struct invoke_evaluate_impl<
		OperationTag, 
		OperationImplsTag, 
		OperandsTag, 
		ContextTag, 
		ChoosenImplTag, 
		hana::when<Condition>
	> : hana::default_ {
		
		template <
			typename Operation,
			typename OperationImpls,
			typename Operands,
			typename Context,
			typename ChoosenImpl
		>
		static constexpr decltype(auto)
		apply(Operation && o, OperationImpls && ois, Operands && os, Context && c, ChoosenImpl && i) {
			/* invoke operation impl. and evaluate the returned value because it might be an expression, 
			 * e.g. if called function is a generic function or e.g. if-then-else. 
			 */
			auto ctx = make_evaluation_context(
				hana::typeid_(HBRS_MPL_FWD(o)),
				hana::transform(HBRS_MPL_FWD(ois), hana::typeid_),
				hana::transform(os, hana::demux(type_of)(unwrap_reference)),
				hana::typeid_(HBRS_MPL_FWD(i)),
				HBRS_MPL_FWD(c)
			);
			/* explicitly create context before unwrapping refs of os because unpack might move objects from os to i */
			return evaluate(
				hana::unpack(
					unwrap_references(HBRS_MPL_FWD(os)), 
					HBRS_MPL_FWD(i)
				),
				ctx
			);
		}
	};
	
	template <
		typename Operation,
		typename OperationImpls,
		typename OperandsTypes,
		typename Context
	>
	constexpr auto
	filter_invokables(Operation && o, OperationImpls && ois, OperandsTypes && ost, Context && c) {
		/* example for ois: std::make_tuple(hana::id) */
		
		decltype(auto) default_constructible =
			hana::filter(
				ois,
				hana::compose(
					hana::trait<std::is_default_constructible>,
					hana::make_type
				)
			);
		
		decltype(auto) non_recursive =
			hana::filter(
				HBRS_MPL_FWD(default_constructible),
				hana::compose(
					hana::not_,
					hana::partial(
						detail::is_recursive_impl,
						o, 
						ost,
						c
					)
				)
			);
		
		/* find impls that are "flat" invokable with operands */
		auto not_invokable_filter = [](auto && i, auto && ost) {
			return hana::unpack(HBRS_MPL_FWD(ost),
				hana::partial(
					hana::trait<std::is_invocable>,
					detail::type_of(HBRS_MPL_FWD(i))
				)
			);
		};
		
		decltype(auto) invokables = 
			hana::filter(
				HBRS_MPL_FWD(non_recursive),
				hana::reverse_partial(not_invokable_filter, ost)
			);
		
		/* find impls that are "deep" invokable with operands, e.g. all functions called within impl are also implemented */
		auto not_implemented_filter = [](auto && ost, auto && i) {
			return hana::unpack(
				HBRS_MPL_FWD(ost),
				hana::partial(
					[](auto && i, auto&&... ost) {
						return decltype(hana::not_(
							is_error_cause(
								HBRS_MPL_FWD(i)(/*wrap_reference*/std::declval<typename std::decay_t<decltype(ost)>::type>()...),
								error_cause::not_implemented{}
							)
						)){};
					}, 
					HBRS_MPL_FWD(i)
				)
			);
		};
		
		decltype(auto) implemented =
			hana::filter(
				HBRS_MPL_FWD(invokables),
				hana::partial(not_implemented_filter, ost)
			);
		
		auto not_recursively_implemented_filter = [](auto && o, auto && ois, auto && ost, auto && c, auto && i) {
			return hana::unpack(
				ost,
				hana::partial(
					[](auto && o, auto && ois, auto && c, auto && i, auto &&... ost) {
						return decltype(
							hana::not_(
								is_error_cause(
									detail::invoke_evaluate(
										HBRS_MPL_FWD(o),
										HBRS_MPL_FWD(ois),
										wrap_references(std::declval<typename std::decay_t<decltype(ost)>::type>()...),
										HBRS_MPL_FWD(c),
										HBRS_MPL_FWD(i)
									),
									error_cause::not_implemented{}
								)
							)
						){};
					},
					HBRS_MPL_FWD(o),
					HBRS_MPL_FWD(ois),
					HBRS_MPL_FWD(c),
					HBRS_MPL_FWD(i)
				)
			);
		};
		
		/* test if evaluate is invokable with impl and operands. this filter removes recursive impls from impl list */
		return hana::filter(
			HBRS_MPL_FWD(implemented),
			hana::partial(
				not_recursively_implemented_filter,
				o, ois, ost, c
			)
		);
	}
	
	
	template <
		typename Error,
		typename Function,
		typename... Arguments,
		typename std::enable_if_t< decltype(is_error(std::declval<Error>())){} >* = nullptr
	>
	constexpr auto
	try_(Error && e, Function && f, Arguments&&... args) {
		return HBRS_MPL_FWD(e);
	}

	template <
		typename T,
		typename Function,
		typename... Arguments,
		typename std::enable_if_t< decltype(!is_error(std::declval<T>())){} >* = nullptr
	>
	constexpr decltype(auto)
	try_(T &&, Function && f, Arguments&&... args) {
		return HBRS_MPL_FWD(f)(HBRS_MPL_FWD(args)...);
	}
	
	//TODO: Implement meaningfull stack trace somewhere!
	template <
		typename Error,
		typename Function,
		typename... Arguments,
		typename std::enable_if_t< decltype(is_error(std::declval<Error>())){} >* = nullptr
	>
	constexpr auto
	trap(Error && e, Function && f, Arguments&&... args) {
		return make_stack_frame(
			HBRS_MPL_FWD(e),
			HBRS_MPL_FWD(f),
			hana::make_tuple(HBRS_MPL_FWD(args)...)
		);
	}

	template <
		typename T,
		typename Function,
		typename... Arguments,
		typename std::enable_if_t< decltype(!is_error(std::declval<T>())){} >* = nullptr
	>
	constexpr decltype(auto)
	trap(T &&, Function && f, Arguments&&... args) {
		return HBRS_MPL_FWD(f)(HBRS_MPL_FWD(args)...);
	}
	
	
	
	HBRS_MPL_DEC_CF4(ask_for_implementation)
	HBRS_MPL_DEF_CF4(ask_for_implementation)

	template <typename OperationTag, typename OperationImplsTag, typename OperandsTypesTag, typename OperandsCRefsTag, bool Condition>
	struct ask_for_implementation_impl<OperationTag, OperationImplsTag, OperandsTypesTag, OperandsCRefsTag, hana::when<Condition>> : hana::default_ {
		
		struct unpack_optional {
			template <
				typename Operation,
				typename OperationImpl,
				typename OperandsTypes
			>
			constexpr decltype(auto)
			operator()(Operation && o, OperationImpl && oi, OperandsTypes && ost) const {
				decltype(auto) impl = HBRS_MPL_FWD(oi).value();
				
				static_assert(decltype(hana::unpack(
						unwrap_references(HBRS_MPL_FWD(ost)),
						hana::partial(hana::trait<std::is_invocable>, hana::type_c<decltype(impl)>)
					)){},
					"choose_implementation must return an callable function object");
				return HBRS_MPL_FWD(impl);
			}
		};
		
		template <
			typename Operation,
			typename OperationImpls,
			typename OperandsTypes,
			typename OperandsCRefs
		>
		static constexpr decltype(auto)
		apply(Operation && o, OperationImpls && ois, OperandsTypes && ost, OperandsCRefs && os) {
			decltype(auto) impl_0or1 = choose_implementation(o, ois, os);
			
			static_assert(
				std::is_same<hana::tag_of_t<decltype(impl_0or1)>, hana::optional_tag>{},
				"choose_implementation must return an hana::optional<T> object"
			);
			
			decltype(auto) impl_eor1 = hana::if_(
				impl_0or1 != hana::nothing,
				HBRS_MPL_FWD(impl_0or1),
				make_error(
					error_cause::not_implemented{},
					make_expression(o, os)
				)
			);
			
			return detail::try_(
				impl_eor1,
				unpack_optional{},
				HBRS_MPL_FWD(o), 
				impl_eor1,
				HBRS_MPL_FWD(ost)
			);
			
				
		}
	};
	
	HBRS_MPL_DEC_CF1(as_const_reference)
	HBRS_MPL_DEF_CF1(as_const_reference)
	
	template <typename TTag, bool Condition>
	struct as_const_reference_impl<TTag, hana::when<Condition>> : hana::default_ {
		template <typename T>
		static constexpr decltype(auto)
		apply(T const& t) { 
			return t;
		}
	};
	
/* namespace detail */ }




template <typename ContextTag>
struct evaluate_impl<expression_tag, ContextTag> {
	template <
		typename Expression, 
		typename Context
	>
	static constexpr decltype(auto)
	apply(Expression && e, Context && c) {
		decltype(auto) o = HBRS_MPL_FWD(e).operation();
		decltype(auto) os = HBRS_MPL_FWD(e).operands();
		
		/* NOTE: Compiler error "note: reference to subobject of 'os_evaluated' is not a constant expression" might
		 *       indicate, that your function implementation might not be constexpr! 
		 */
		/* NOTE: Compiler error "
		 *         error: constexpr variable '...' must be initialized by a constant expression
		 *           ...
		 *         note: read of variable whose lifetime has ended
		 *           ...
		 *         note: declared here
		 *           decltype(auto) os_evaluated = hana::transform(
		 *       " might indicate, that your function implementation might return a reference to a temporary object, 
		 *       e.g. transform 
		 *         struct ... { auto & data() { ... } }
		 *         ~> 
		 *         struct ... { auto & data() & { ... }, auto && data() && { ... } }
		 *         or
		 *         struct ... { decltype(auto) data() { ... } }
		 */
		decltype(auto) os_evaluated = hana::transform(
			HBRS_MPL_FWD(os),
			/* evaluate might return lvalue references that must be wrapped e.g. evaluate(forward(forward(int&))) */
			hana::demux(wrap_reference)( hana::reverse_partial(evaluate, c) )
		);
		/* NOTE: do not use 'os' resp. 'e.operands()' beginning from here, 
		 * because they (if rvalues) might have been moved to 'os_evaluated'!
		 */
		
		decltype(auto) os_evaluated_types = hana::transform(
			HBRS_MPL_FWD(os_evaluated), hana::demux(detail::type_of)(unwrap_reference)
		);
		
		decltype(auto) impls_0n = detail::filter_invokables(
			o,
			implementations_of(o),
			os_evaluated_types,
			c
		);
		/* NOTE: filter_invokables might return an not_implemented error/stack_frame that is not default constructible, 
		 * because its context may contain references. 
		 * NO LONGER TRUE, because context just contains hana::type<> instances!
		 */
		
		decltype(auto) impls_1n = [](auto && o, auto && os_evaluated, auto && impls_0n){
			/* NOTE: use of constexpr if here is required because we only want to move objects from os_evaluated 
			 * to make_error in case of an error
			 */
			if constexpr (decltype(hana::is_empty(HBRS_MPL_FWD(impls_0n))){}) {
				return make_error(
					error_cause::not_implemented{},
					make_expression(HBRS_MPL_FWD(o), HBRS_MPL_FWD(os_evaluated))
				);
			} else {
				return HBRS_MPL_FWD(impls_0n);
			}
		}(o, os_evaluated, HBRS_MPL_FWD(impls_0n));
		
		decltype(auto) os_evaluated_crefs = detail::transform_and_forward(
			os_evaluated,
			hana::demux(detail::as_const_reference)(unwrap_reference)
		);
		
		decltype(auto) impl = detail::try_(
			impls_1n,
			detail::ask_for_implementation,
			o, 
			impls_1n,
			os_evaluated_types,
			HBRS_MPL_FWD(os_evaluated_crefs) // pass const references to prevent manipulation
		);
		
		return detail::try_(
			impl,
			detail::invoke_evaluate,
			o,
			implementations_of(o),
			HBRS_MPL_FWD(os_evaluated),
			c,
			impl
		);
	}
};

template <typename ContextTag>
struct evaluate_impl<detail::annotated_tag, ContextTag> {
	
	template <
		typename Annotated, 
		typename Context,
		typename std::enable_if_t< decltype( has_annotation( std::declval<Annotated>(), delay_evaluation{} ) ){} >* = nullptr
	>
	static constexpr auto
	apply(Annotated && at, Context &&) {
		return remove_annotation(HBRS_MPL_FWD(at), delay_evaluation{});
	}
	
	template <
		typename Annotated, 
		typename Context,
		typename std::enable_if_t< 
			decltype( !has_annotation( std::declval<Annotated>(), delay_evaluation{} ) ){}
			&& std::is_reference<Annotated>::value
		>* = nullptr
	>
	static constexpr auto
	apply(Annotated && at, Context &&) {
		return HBRS_MPL_FWD(at);
	}
	
	template <
		typename Annotated, 
		typename Context,
		typename std::enable_if_t< 
			decltype( !has_annotation( std::declval<Annotated>(), delay_evaluation{} ) ){}
			&& !std::is_reference<Annotated>::value
		>* = nullptr
	>
	static constexpr auto
	apply(Annotated && at, Context &&) {
		return HBRS_MPL_FWD(at);
	}
	
};

template <typename ExpressionTag, typename ContextTag, bool Condition>
struct evaluate_impl<ExpressionTag, ContextTag, hana::when<Condition>> : hana::default_ {
	template <
		typename Value, 
		typename Context,
		typename std::enable_if_t< std::is_rvalue_reference<Value&&>::value >* = nullptr
	>
	static constexpr auto
	/* When using decltype(auto) as return type then, if Value is not a reference (Value without & or &&), 
	 * this function returns a rvalue reference. But after calling this function Value is destroyed and 
	 * rvalue reference is dangling?!
	 * 
	 * Using auto (without decltype) as return type forces move construction and prevents perfect forwarding. 
	 * But this is okay because rvalue_reference_wrapper forces move construction, too.
	 */
	apply(Value && v, Context &&) {
		return HBRS_MPL_FWD(v);
	}
	
	template <
		typename Value, 
		typename Context,
		typename std::enable_if_t< std::is_lvalue_reference<Value&&>::value >* = nullptr
	>
	static constexpr decltype(auto)
	apply(Value && v, Context &&) {
		return HBRS_MPL_FWD(v);
	}
	
	template <
		typename Value, 
		typename Context,
		typename std::enable_if_t< !std::is_reference<Value&&>::value >* = nullptr
	>
	static constexpr auto
	apply(Value && v, Context &&) = delete;  //IS THIS ACTUALLY CALLABLE?!?
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_EVALUATE_IMPL_HPP
