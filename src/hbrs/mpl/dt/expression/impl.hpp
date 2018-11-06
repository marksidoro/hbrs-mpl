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

#ifndef HBRS_MPL_DT_EXPRESSION_IMPL_HPP
#define HBRS_MPL_DT_EXPRESSION_IMPL_HPP

#include "fwd.hpp"

//TODO: Do not include impls here because else you will run into include cycles problems!
#include <hbrs/mpl/fwd/fn/preincrement.hpp>
#include <hbrs/mpl/fwd/fn/predecrement.hpp>
#include <hbrs/mpl/fwd/fn/postincrement.hpp>
#include <hbrs/mpl/fwd/fn/postdecrement.hpp>
#include <hbrs/mpl/fwd/fn/plus.hpp>
#include <hbrs/mpl/fwd/fn/minus.hpp>
#include <hbrs/mpl/fwd/fn/multiply.hpp>
#include <hbrs/mpl/fwd/fn/divide.hpp>
#include <hbrs/mpl/fwd/fn/greater_equal.hpp>
#include <hbrs/mpl/fwd/fn/invoke.hpp>
#include <hbrs/mpl/fwd/fn/at.hpp>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>

#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <
	typename Operation,
	typename Operands
>
struct expression {
public:
	
	template<
		typename Operation_ = Operation,
		typename Operands_ = Operands,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<Operation, Operation_> &&
			detail::is_braces_constructible_v<Operands, Operands_>
		>* = nullptr
	>
	constexpr
	expression(Operation_ && o, Operands_ && os)
	:
	operation_{HBRS_MPL_FWD(o)},
	operands_{HBRS_MPL_FWD(os)} {}
	
	/* ATTENTION: copy construction may result in redundant operations, e.g.
	 * 
	 * auto f(X a) {
	 *   auto b = g(a); //copy ctor called here
	 *   auto c = h(b);
	 *   auto d = i(b);
	 *   return j(c,d);
	 * }
	 * 
	 * syntax tree for f(x1) := j(h(g(a)), i(g(a)));
	 * => term 'g(a)' computed twice
	 * 
	 * 
	 * NOTE: Above code can be replaced by (only if f1 and f2 are lazy!):
	 * 
	 * auto f1(X a) {
	 *   return g(a);
	 * }
	 * 
	 * auto f2(X b) {
	 *   return j(h(b), i(b));
	 * }
	 * 
	 * f2(f1(x1));
	 * 
	 * 
	 * NOTE: It is not possible to programmatically identify at compile time that both terms 'g(a)' in 
	 *       j(h(g(a)), i(g(a))) are equal, because at compile time only equivalence of types (but not values) can be 
	 *       decided. But equivalence of types is not enough to identify redundant operations, consider this example:
	 * 
	 * auto f(X a, X b) {
	 *   return j(g(a), g(b));
	 * }
	 * 
	 * Both terms 'g(a)' and 'g(b)' share the same expression type but might produce different values.
	 * 
	 * TODO: Move possible workarounds to separate file?!
	 * NOTE: Possible solutions / workarounds:
	 * a.) call f with annotated types, X ~> W<X,N> where N is a unique number for each arg. 
	 *     W<X,N> is a wrapper type that behaves like X. all member functions and operators of W<X,N> point to impls 
	 *     in X, e.g. because X <|-- W<X,N> or because W<X,N> overloads dot-operator (in future C++ revisions).
	 *     Because N is known at compile-time we can identify redundant operations by syntax tree traversal.
	 * 
	 *     Disadvantages: 
	 *      - Only if f is generic. Does not work when f takes pod like 'int' because in C++ it is not possible to inherit from pod.
	 *      - if f uses e.g. tag dispatching (like in boost hana) to enable impls then W<X,N> must deal with that. 
	 *        W<X,N> is not a generic solution for yet-unknown tag dispatching techniques that impls might use in future.
	 * 
	 * b.) lambdas with positional arguments, e.g. f = j(h(_1), i(_1)) (g(h))
	 *     Disadvantages:
	 *      - non-intuitive syntax
	 *      - requires explicit reformulation of algorithm by programmer
	 *      - reimplementation of lambdas, c++ lambdas cannot be used because of eager evaluation
	 */
	
	[[deprecated("copy construction may result in redundant operations")]] /* TODO: Replace 'deprecated' with 'warning' once available */
	constexpr
	expression(expression const&) = default;
	
	constexpr
	expression(expression &&) = default;
	
	template<
		typename Operation_, typename Operands_,
		typename std::enable_if_t< 
			std::is_constructible_v<Operation, Operation_&&> && std::is_constructible_v<Operands, Operands_&&>
		>* = nullptr
	>
	constexpr
	expression(expression<Operation_,Operands_>&& other)
	: expression(HBRS_MPL_FWD(other).operation_, HBRS_MPL_FWD(other).operands_) {}
	
	template<
		typename Operation_, typename Operands_,
		typename std::enable_if_t< 
			std::is_constructible_v<Operation, Operation_ const&> && std::is_constructible_v<Operands, Operands_ const&>
		>* = nullptr
	>
	constexpr
	expression(expression<Operation_,Operands_> const& other)
	: expression(other.operation_, other.operands_) {}
	
	[[deprecated("copy assignment may result in redundant operations")]] /* TODO: Replace 'deprecated' with 'warning' once available */
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
	
	constexpr decltype(auto) 
	operator++() && { return preincrement(std::move(*this)); }
	constexpr decltype(auto)
	operator--() && { return predecrement(std::move(*this)); }
	constexpr decltype(auto)
	operator++(int) && { return postincrement(std::move(*this)); }
	constexpr decltype(auto)
	operator--(int) && { return postdecrement(std::move(*this)); }
	
	template<typename Expression>
	constexpr decltype(auto)
	operator+(Expression && e) && { return plus(std::move(*this), HBRS_MPL_FWD(e)); }
	
	template<typename Expression>
	constexpr decltype(auto)
	operator-(Expression && e) && { return minus(std::move(*this), HBRS_MPL_FWD(e)); }
	
	template<typename Expression>
	constexpr decltype(auto)
	operator*(Expression && e) && { return multiply(std::move(*this), HBRS_MPL_FWD(e)); }
	
	template<typename Expression>
	constexpr decltype(auto)
	operator/(Expression && e) && { return divide(std::move(*this), HBRS_MPL_FWD(e)); }
	
	template<typename Expression>
	constexpr decltype(auto)
	operator>=(Expression && e) && { return greater_equal(std::move(*this), HBRS_MPL_FWD(e)); }
	
	template<typename... Expressions>
	constexpr decltype(auto)
	operator()(Expressions && ... es) && { return invoke(std::move(*this), HBRS_MPL_FWD(es)...); }
	
	template<typename Expression>
	constexpr decltype(auto)
	operator[](Expression && e) && { return at(std::move(*this), HBRS_MPL_FWD(e)); }
	
	//TODO: Add more operators!
	
	
	
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

#include <hbrs/mpl/core/reference_wrapper/fwd.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template <>
struct wrap_reference_impl<expression_tag> {
	template <typename T>
	static constexpr auto
	apply(T && t) {
		return HBRS_MPL_FWD(t);
	}
};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DT_EXPRESSION_IMPL_HPP
