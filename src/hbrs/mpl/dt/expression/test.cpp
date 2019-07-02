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

#define BOOST_TEST_MODULE expression_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

//TODO: Move if, apply and invoke to their own test files!

#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/core/evaluate.hpp>
#include <hbrs/mpl/fn/if.hpp>
#include <hbrs/mpl/fn/apply.hpp>
#include <hbrs/mpl/fn/invoke.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/ext/std/array.hpp>
#include <boost/hana/back.hpp>
#include <boost/hana/plus.hpp>
#include <vector>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

struct not_copyable {
	constexpr 
	not_copyable() {}
	
	not_copyable(not_copyable const&) = delete;
	not_copyable(not_copyable &&) = default;
	
	not_copyable& 
	operator=(not_copyable const&) = delete;
	not_copyable& 
	operator=(not_copyable &&) = default;
};

struct op1_impl1 {
	constexpr decltype(auto)
	operator()(hana::int_<1337>) const {
		return hana::int_c<42>;
	}
};

struct op1_t {
	template <typename _0t>
	constexpr decltype(auto)
	operator()(_0t&&) const;
};


template <>
struct implementations_of_impl<op1_t> {
	static constexpr auto
	apply(op1_t) {
		return hana::make_tuple(op1_impl1{});
	}
};

constexpr op1_t op1{};

template <typename _0t>
constexpr decltype(auto)
op1_t::operator()(_0t&& _0) const {
	return call(op1, HBRS_MPL_FWD(_0));
}


struct op2_impl1 {
	constexpr decltype(auto)
	operator()(hana::int_<1337>) const {
		return op1(hana::int_c<42>); /* not implemented */
	}
};

struct op2_t {
	template <typename _0t>
	constexpr decltype(auto)
	operator()(_0t&&) const;
};


template <>
struct implementations_of_impl<op2_t> {
	static constexpr auto
	apply(op2_t) {
		return hana::make_tuple(op2_impl1{});
	}
};

constexpr op2_t op2{};

template <typename _0t>
constexpr decltype(auto)
op2_t::operator()(_0t&& _0) const {
	return call(op2, HBRS_MPL_FWD(_0));
}

struct op3_impl1 {
	constexpr decltype(auto)
	operator()(hana::int_<1337>) const {
		return op1(hana::int_c<1337>);
	}
};

struct op3_t {
	template <typename _0t>
	constexpr decltype(auto)
	operator()(_0t&&) const;
};


template <>
struct implementations_of_impl<op3_t> {
	static constexpr auto
	apply(op3_t) {
		return hana::make_tuple(op3_impl1{});
	}
};

constexpr op3_t op3{};

template <typename _0t>
constexpr decltype(auto)
op3_t::operator()(_0t&& _0) const {
	return call(op3, HBRS_MPL_FWD(_0));
}






struct op6_impl1 {
	constexpr decltype(auto)
	operator()(hana::int_<1337>) const;
};

struct op6_impl2 {
	constexpr decltype(auto)
	operator()(hana::int_<1337>) const;
};

struct op6_t {
	template <typename _0t>
	constexpr decltype(auto)
	operator()(_0t&&) const;
};


template <>
struct implementations_of_impl<op6_t> {
	static constexpr auto
	apply(op6_t) {
		return hana::make_tuple(op6_impl1{} /* , op6_impl2{} */);
	}
};

constexpr op6_t op6{};

template <typename _0t>
constexpr decltype(auto)
op6_t::operator()(_0t&& _0) const {
	return call(op6, HBRS_MPL_FWD(_0));
}

struct op7_impl1 {
	constexpr decltype(auto)
	operator()(hana::int_<1337>) const;
};

struct op7_impl2 {
	constexpr decltype(auto)
	operator()(hana::int_<1337>) const;
};

struct op7_t {
	template <typename _0t>
	constexpr decltype(auto)
	operator()(_0t&&) const;
};


template <>
struct implementations_of_impl<op7_t> {
	static constexpr auto
	apply(op7_t) {
		return hana::make_tuple(op7_impl1{}, op7_impl2{});
	}
};

constexpr op7_t op7{};

template <typename _0t>
constexpr decltype(auto)
op7_t::operator()(_0t&& _0) const {
	return call(op7, HBRS_MPL_FWD(_0));
}

constexpr decltype(auto)
op6_impl1::operator()(hana::int_<1337>) const {
	return op7(hana::int_c<1337>);
}

constexpr decltype(auto)
op6_impl2::operator()(hana::int_<1337>) const {
	return hana::int_c<42>;
}

constexpr decltype(auto)
op7_impl1::operator()(hana::int_<1337>) const {
	return op6(hana::int_c<1337>);
}

constexpr decltype(auto)
op7_impl2::operator()(hana::int_<1337>) const {
	return hana::int_c<666>;
}





struct op8_impl1 {
	
	template<
		typename T, 
		typename Allocator, 
		typename E, 
		typename std::enable_if_t< 
			std::is_same< std::decay_t<T>, std::decay_t<E> >{}
		>* = nullptr
	>
	std::vector<T, Allocator> &
	operator()(std::vector<T, Allocator> & v, E && e) const {
		v.push_back(HBRS_MPL_FWD(e));
		return v;
	}
	
};

struct op8_t {
	template <typename S, typename E>
	constexpr decltype(auto)
	operator()(S&&, E&&) const;
};


template <>
struct implementations_of_impl<op8_t> {
	static constexpr auto
	apply(op8_t) {
		return hana::make_tuple(op8_impl1{});
	}
};

constexpr op8_t op8{};

template <typename S, typename E>
constexpr decltype(auto)
op8_t::operator()(S&& s, E&& e) const {
	return call(op8, HBRS_MPL_FWD(s), HBRS_MPL_FWD(e));
}


struct op9_impl1 {
	template<
		typename T, 
		typename Allocator, 
		typename E, 
		typename std::enable_if_t< std::is_same< std::decay_t<T>, std::decay_t<E> >{} >* = nullptr
	>
	decltype(auto)
	operator()(std::vector<T, Allocator> & v, E && e) const {
		return make_error(1,1);
	}
	
	template<
		typename T, 
		typename Allocator, 
		typename E, 
		typename std::enable_if_t< std::is_same< std::decay_t<T>, std::decay_t<E> >{} >* = nullptr
	>
	auto
	operator()(std::vector<T, Allocator> const& v, E && e) const {
		std::vector<T, Allocator> v_{v};
		v_.push_back(HBRS_MPL_FWD(e));
		return v_;
	}
};

struct op9_t {
	template <typename S, typename E>
	constexpr decltype(auto)
	operator()(S&&, E&&) const;
};


template <>
struct implementations_of_impl<op9_t> {
	static constexpr auto
	apply(op9_t) {
		return hana::make_tuple(op9_impl1{});
	}
};

constexpr op9_t op9{};

template <typename S, typename E>
constexpr decltype(auto)
op9_t::operator()(S&& s, E&& e) const {
	return call(op9, HBRS_MPL_FWD(s), HBRS_MPL_FWD(e));
}



struct op12_impl1 {
	
	template<
		typename T, 
		typename Allocator, 
		typename E, 
		typename std::enable_if_t< 
			std::is_same< std::decay_t<T>, std::decay_t<E> >{} 
			&& std::is_rvalue_reference<E&&>::value
		>* = nullptr
	>
	std::vector<T, Allocator> &
	operator()(std::vector<T, Allocator> & v, E && e) const {
		v.push_back(HBRS_MPL_FWD(e));
		return v;
	}
	
};

struct op12_t {
	template <typename S, typename E>
	constexpr decltype(auto)
	operator()(S&&, E&&) const;
};


template <>
struct implementations_of_impl<op12_t> {
	static constexpr auto
	apply(op12_t) {
		return hana::make_tuple(op12_impl1{});
	}
};

constexpr op12_t op12{};

template <typename S, typename E>
constexpr decltype(auto)
op12_t::operator()(S&& s, E&& e) const {
	return call(op12, HBRS_MPL_FWD(s), HBRS_MPL_FWD(e));
}






namespace detail {

	struct id_impl1 {
		template<typename T>
		constexpr auto
		operator()(T&& t) const {
			return HBRS_MPL_FWD(t);
		}
	};

/* namespace detail */ }

struct id_t {
	template <typename T>
	constexpr decltype(auto)
	operator()(T&&) const;
};

template <>
struct implementations_of_impl<id_t> {
	static constexpr auto
	apply(id_t) {
		return hana::make_tuple(detail::id_impl1{});
	}
};

constexpr id_t id{};

template <typename T>
constexpr decltype(auto)
id_t::operator()(T&& t) const {
	return call(id, HBRS_MPL_FWD(t));
}





namespace detail {

	struct forward_impl1 {
		template<typename T>
		constexpr decltype(auto)
		operator()(T&& t) const {
			return HBRS_MPL_FWD(t);
		}
	};

/* namespace detail */ }

struct forward_t {
	template <typename T>
	constexpr decltype(auto)
	operator()(T&&) const;
};

template <>
struct implementations_of_impl<forward_t> {
	static constexpr auto
	apply(forward_t) {
		return hana::make_tuple(detail::forward_impl1{});
	}
};

constexpr forward_t forward{};

template <typename T>
constexpr decltype(auto)
forward_t::operator()(T&& t) const {
	return call(forward, HBRS_MPL_FWD(t));
}









struct op13_impl1 {
	template<typename T>
	int
	operator()(T&&) const {
		return 1;
	}
};

struct op13_impl2 {
	template<typename T>
	int
	operator()(T const&) const {
		return 2;
	}
};

struct op13_t {
	template <typename T>
	constexpr decltype(auto)
	operator()(T&&) const;
};


template <>
struct implementations_of_impl<op13_t> {
	static constexpr auto
	apply(op13_t) {
		return hana::make_tuple(op13_impl1{}, op13_impl2{});
	}
};

constexpr op13_t op13{};

template <typename T>
constexpr decltype(auto)
op13_t::operator()(T&& t) const {
	return call(op13, HBRS_MPL_FWD(t));
}


struct to_std_function {
	template<typename F, typename... Args>
	auto
	operator()(F && f, Args&&... args) const {
		typedef decltype(
			HBRS_MPL_FWD(f)(HBRS_MPL_FWD(args)...)
		) R;
		
		return std::function<R(Args&&...)>{HBRS_MPL_FWD(f)};
	}
};

struct make_array_t {
	/* Src: https://stackoverflow.com/a/16950454/6490710 */
	template <typename... Ts>
	constexpr std::array<std::common_type_t<Ts...>, sizeof...(Ts)>
	operator()(Ts &&... ts) const {
		return {{HBRS_MPL_FWD(ts)...}};
	}
};

constexpr make_array_t make_array{};

template <typename OperationImplsTag, typename OperandsTag, bool Condition>
struct choose_implementation_impl<op13_t, OperationImplsTag, OperandsTag, hana::when<Condition>> : hana::default_ {
	
	template <
		typename Operation,
		typename OperationImpls,
		typename Operands
	>
	/* function is not constexpr because we make an array of std::function objects and those are not constexpr constructible */
	static auto
	apply(Operation const& o, OperationImpls && ois, Operands && os) {
		
		//compose is necessary to not unwrap references to hana::partial, 
		//because hana::partial stores arguments and thus drops the "is reference" status.
		auto fos = hana::transform(
			hana::transform(HBRS_MPL_FWD(ois), hana::partial(hana::partial, to_std_function{})),
			hana::partial(hana::compose(hana::unpack, unwrap_references), HBRS_MPL_FWD(os))
		);
		
		static_assert(decltype(
			hana::all_of(hana::transform(fos, hana::make_type), hana::equal.to(hana::make_type(hana::front(fos))))
		){}, "all implementations must have a compatible signature");		
		
		auto fosa = hana::unpack(fos, make_array);
		
		static_assert(decltype(hana::length(fosa))::value == 2, "");
		
		return hana::make_optional(HBRS_MPL_FWD(fosa)[1]);
	}
};

HBRS_MPL_NAMESPACE_END


struct op14_impl1 {
	constexpr void
	operator()() const {}
};

struct op14_impl2 {
	constexpr void
	operator()(int) const {}
};

struct op14_impl3 {
	template<typename T>
	constexpr void
	operator()(T) const {}
};

constexpr void 
op14_impl4() {}

constexpr void 
op14_impl5(int) {}




HBRS_MPL_NAMESPACE_BEGIN

struct op15_impl {
	template<typename T>
	constexpr decltype(auto)
	operator()(T&& t) const {
		return HBRS_MPL_FWD(t);
	}
};

struct op15_t {
	template <typename T>
	constexpr decltype(auto)
	operator()(T&&) const;
};


template <>
struct implementations_of_impl<op15_t> {
	static constexpr auto
	apply(op15_t) {
		return hana::make_tuple(op15_impl{});
	}
};

constexpr op15_t op15{};

template <typename T>
constexpr decltype(auto)
op15_t::operator()(T&& t) const {
	return call(op15, HBRS_MPL_FWD(t));
}

HBRS_MPL_NAMESPACE_END






namespace hana = boost::hana;


struct t1{
	constexpr hana::true_
	operator==(t1 const&) const { return {}; }

	constexpr hana::false_
	operator!=(t1 const&) const { return {}; }
};
struct t2{
	constexpr hana::true_
	operator==(t2 const&) const { return {}; }

	constexpr hana::false_
	operator!=(t2 const&) const { return {}; }
};

template<typename T>
constexpr decltype(auto)
op4(hana::int_<1337>, T) {
	return hana::int_c<42>;
}

template<typename T>
constexpr decltype(auto)
op5(hana::int_<1337>, T) {
	return hana::int_c<42>;
}

constexpr decltype(auto)
op10(hana::int_<1337> const&) {
	return hana::int_c<666>;
}

constexpr decltype(auto)
op10(hana::int_<1337> &&) {
	return hana::int_c<42>;
}

std::vector<int> &
op10(std::vector<int> & v, int && e) {
	v.push_back(HBRS_MPL_FWD(e));
	return v;
}

struct op11 {
	template<typename T>
	T &
	operator()(T & v, int && e) const {
		v.push_back(HBRS_MPL_FWD(e));
		return v;
	}
};

struct t5 {
	constexpr
	t5() {};
	
	constexpr
	t5(t5 const&) = default;
	
	constexpr
	t5(t5 &&) = delete;
};

struct t6 {
	constexpr
	t6() {};
	
	constexpr
	t6(t6 const&) = delete;
	
	constexpr
	t6(t6 &&) = default;
};

struct t8 {
	t8() : copy_nr{0}, move_nr{0} {};
	t8(t8 const& o) : copy_nr{o.copy_nr+1}, move_nr{o.move_nr} {};
	t8(t8 && o) : copy_nr{o.copy_nr}, move_nr{o.move_nr+1} {};
	
	std::size_t copy_nr;
	std::size_t move_nr;
};

struct t7 {
	t7() : valid{true} {}
	
	t7(t7 const&) = default;
	t7(t7 && o) : valid{o.release()} {};
	
	bool release() {
		bool was_valid = valid;
		valid = false;
		return was_valid;
	}
	
	bool valid;
};

BOOST_AUTO_TEST_SUITE(expression_test)

BOOST_AUTO_TEST_CASE(expression_test_1) {
	using namespace hbrs::mpl;
	namespace hana = boost::hana;
	using namespace hana::literals;
	using detail::annotated;
	using detail::make_annotated;
	
	static constexpr auto is_invocable = [](auto && f, auto &&... xs) {
		return hana::bool_c<
			std::is_invocable_v<decltype(f), decltype(xs)...>
		>;
	};
	
	constexpr t6 s77 = hana::at_c<0>(hana::id(
		make_expression(1, hana::make_tuple(t6{})).operands()
	));
	
	constexpr auto s1 = make_expression(not_copyable{},not_copyable{});
	
	constexpr hana::tuple<op1_impl1> s18 = implementations_of(op1);
	constexpr hana::tuple<op1_impl1> s19 = implementations_of(op1_t{});
	
	//TODO: Expression or Value?
// 	constexpr expression<
// 		op1_t,
// 		hana::tuple< detail::lvalue_reference_wrapper< hana::int_<1337> const > >
// 	> s2 = op1(hana::int_c<1337>);
	
	//TODO: Expression or Value?
// 	static_assert(std::is_same<
// 		decltype(op1(hana::int_<1337>{})),
// 		expression<
// 			op1_t,
// 			hana::tuple<
// 				detail::rvalue_reference_wrapper< hana::int_<1337> > 
// 			>
// 		>
// 	>{}, "");

	constexpr hana::int_<42> s3 = evaluate(op1(hana::int_c<1337>));
	
	constexpr hana::int_<42> s66 = evaluate(op1(hana::int_<1337>{}));

	constexpr hana::int_<42> s43 = op10(hana::at(hana::make_tuple(hana::int_c<1337>), hana::int_c<0>));
	
	//TODO: Expression or Value?
// 	constexpr expression<
// 		op1_t,
// 		hana::tuple< 
// 			detail::lvalue_reference_wrapper< hana::int_<1337> const >
// 		>
// 	> s7 = evaluate(add_annotation(op1(hana::int_c<1337>), delay_evaluation{}));
	

	//TODO: Expression or Value?
// 	static_assert(std::is_same<
// 		decltype(
// 			add_annotation(op1(hana::int_<1337>{}), delay_evaluation{})
// 		),
// 		annotated<
// 			expression<
// 				op1_t,
// 				hana::tuple<
// 					detail::rvalue_reference_wrapper< hana::int_<1337> >
// 				>
// 			>,
// 			hana::tuple<
// 				delay_evaluation
// 			>
// 		>
// 	>{}, "");
	
	//TODO: Expression or Value?
// 	static_assert(std::is_same<
// 		decltype(
// 			evaluate(add_annotation(op1(hana::int_<1337>{}), delay_evaluation{}))
// 		),
// 		expression<
// 			op1_t,
// 			hana::tuple<
// 				detail::rvalue_reference_wrapper< hana::int_<1337> >
// 			>
// 		>
// 	>{}, "");

	
	
	
	
	
	
	
	
	static_assert(std::is_same<
		decltype(evaluate(unwrap_reference(wrap_reference(hana::front)))),
		hana::front_t const&
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate(unwrap_reference(wrap_reference(hana::make_tuple(hana::int_c<1337>))))),
		hana::tuple< hana::int_<1337> >
	>{}, "");
	
	
	
	
	static_assert(std::is_same<
		decltype(add_annotation(invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), delay_evaluation{})),
		annotated<
			expression<
				invoke_t, 
				hana::tuple<
					detail::lvalue_reference_wrapper<const hana::front_t>, 
					detail::rvalue_reference_wrapper< 
						hana::tuple<
							hana::int_<1337> 
						> 
					>
				> 
			>, 
			hana::tuple<delay_evaluation> 
		>
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate(add_annotation(invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), delay_evaluation{}))),
		expression<
			invoke_t, 
			hana::tuple<
				detail::lvalue_reference_wrapper<const hana::front_t>,
				detail::rvalue_reference_wrapper< 
					hana::tuple<
						hana::int_<1337> 
					> 
				>
			> 
		>
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate(evaluate(add_annotation(invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), delay_evaluation{})))),
		hana::int_<1337>
	>{}, "");
	
	static_assert(std::is_same<
		decltype(wrap_reference(evaluate(evaluate(add_annotation(invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), delay_evaluation{}))))),
		detail::rvalue_reference_wrapper< hana::int_<1337> >
	>{}, "");
	
// 	static_assert(std::is_same<
// 		decltype(detail::evaluate_phase2(
// 			if_, 
// 			hana::make_tuple(
// 				hana::true_c, 
// 				invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), 
// 				hana::false_c
// 			),
// 			make_no_context(), 
// 			hana::tag_of_t<if_impl1>{}
// 		)),
// 		hana::int_<1337>
// 	>{}, "");
	
// 	static_assert(std::is_same<
// 		decltype(detail::evaluate_phase2(
// 			if_, 
// 			hana::make_tuple(
// 				hana::true_c, 
// 				evaluate(add_annotation(invoke(hana::front, hana::make_tuple(hana::int_c<1337>)), delay_evaluation{})), 
// 				hana::false_c
// 			),
// 			make_no_context(), 
// 			hana::tag_of_t<if_impl1>{}
// 		)),
// 		hana::int_<1337>
// 	>{}, "");
	
	
	//TODO: Rework for new evaluate function
	constexpr auto s40 = op2(hana::int_c<1337>);
	constexpr auto s41 = evaluate(s40); /* s41 is error because op2 calls op1 with not-implemented type */
	
	//TODO: Rework for new evaluate function
// 	constexpr auto s11 = evaluate(op2(hana::int_c<1337>));
// 	static_assert(decltype(is_error(s11)){},"");
// 	static_assert(hana::make_type(s11.error().error().cause()) == hana::type_c<error_cause::not_implemented>, "");
	
	

	constexpr hana::int_<42> s12 = evaluate(op3(hana::int_c<1337>));


	
	
	
// 	static_assert(&op4<int> == &op4<int>, "");
// 	constexpr auto s13 = &op4<int>;
// 	constexpr auto s14 = &op5<int>;
// 	static_assert(std::is_same<decltype(s13), decltype(s14)>{}, "");
	
	//TODO: Rework recursion detection for new evaluate!
	constexpr auto s15 = op6(hana::int_c<1337>);
	constexpr hana::int_<666> s16 = evaluate(op6(hana::int_c<1337>));
	hana::int_<666> s20 = evaluate(op6(hana::int_c<1337>));
	
	
	
	std::vector<int> s21{1,2,3};
	s21.push_back(4);
	BOOST_CHECK(s21.at(3) == 4);
	std::vector<int> s22{1,2,3};
	std::vector<int> const s62{1,2,3};
	
	constexpr hana::tuple<op8_impl1> s27 = implementations_of(op8_t{});
	
	static_assert(decltype(
		!detail::is_recursive_impl(op8, std::make_tuple(s22, 4), make_no_context(), hana::type_c<op8_impl1>)
		){}, "");
	
	hana::true_ s31 = is_invocable(op8_impl1{}, s22, 4);
	
	hana::true_ s32 = hana::unpack(
		hana::tuple<op8_impl1, std::vector<int>&, int>(op8_impl1{}, s22, 4),
		is_invocable
	);
	
	static_assert(is_invocable(op1_impl1{}, hana::int_c<1337>) == hana::true_c, "");
	static_assert(is_invocable(op1_impl1{}, hana::int_c<42>) == hana::false_c, "");
	
	
	hana::tuple<detail::lvalue_reference_wrapper< std::vector<int> >, int> s35 = wrap_references(s22, 4);
	BOOST_ASSERT(& (unwrap_reference(hana::at_c<0>(s35))) == &s22);
	
	hana::tuple<op8_impl1, std::vector<int>&, int> s34 = unwrap_references(hana::make_tuple(op8_impl1{}, std::ref(s22), 4));
	
	hana::true_ s33 = hana::unpack(
		unwrap_references(hana::make_tuple(op8_impl1{}, detail::make_lvalue_reference_wrapper(s22), 4)),
		is_invocable
	);
	
	hana::true_ s36 = hana::unpack(
		detail::forward_as_tuple(op8_impl1{}, s22, 4),
		is_invocable
	);
	
	std::vector<int> & s25 = detail::invoke_evaluate(
		op8, 
		implementations_of(op8), 
		wrap_references(s22, 4),
		make_no_context(), 
		op8_impl1{}
	);
	
	static_assert(std::is_same<
		decltype(
			detail::invoke_evaluate(
				op8, 
				implementations_of(op8), 
				wrap_references(s22, 4),
				make_no_context(), 
				op8_impl1{}
			)
		),
		std::vector<int> &
	>{}, "");
	
	BOOST_CHECK(
		wrap_reference(
			hana::unpack(
				unwrap_references(wrap_references(4)), 
				hana::id
			)
		) == 4
	);
	
	s22.resize(3);
	BOOST_CHECK(
		detail::invoke_evaluate(
			op8, 
			implementations_of(op8), 
			wrap_references(s22, 4),
			make_no_context(), 
			op8_impl1{}
		).at(3) == 4
	);
	
// 	static_assert(std::is_same<
// 		decltype(detail::evaluate_phase2(
// 			op8, 
// 			wrap_references(s22, 4),
// 			make_no_context(), 
// 			hana::tag_of_t<op8_impl1>{}
// 		)),
// 		std::vector<int> &
// 	>{}, "");
// 	
// 	BOOST_CHECK(
// 		&(detail::evaluate_phase2(
// 			op8, 
// 			wrap_references(s22, 4),
// 			make_no_context(), 
// 			hana::tag_of_t<op8_impl1>{}
// 		)) == &s22
// 	);
// 	
// 	BOOST_CHECK(
// 		&(detail::evaluate_phase2(
// 			op8, 
// 			hana::make_tuple(evaluate(wrap_reference(unwrap_reference(wrap_reference(s22)))), evaluate(wrap_reference(4))),
// 			make_no_context(), 
// 			hana::tag_of_t<op8_impl1>{}
// 		)) == &s22
// 	);
// 	
// 	BOOST_CHECK(
// 		&(detail::evaluate_phase2(
// 			op8, 
// 			hana::transform(
// 				wrap_references(s22,4),
// 				hana::reverse_partial(evaluate, make_no_context())
// 			),
// 			make_no_context(), 
// 			hana::tag_of_t<op8_impl1>{}
// 		)) == &s22
// 	);
// 	
// 	s22.resize(3);
// 	BOOST_CHECK(
// 		detail::evaluate_phase2(
// 			op8, 
// 			hana::transform(
// 				wrap_references(s22,4),
// 				hana::reverse_partial(evaluate, make_no_context())
// 			),
// 			make_no_context(), 
// 			hana::tag_of_t<op8_impl1>{}
// 		).at(3) == 4
// 	);
	
	BOOST_CHECK(
		&(wrap_reference(
			hana::unpack(
				unwrap_references(
					wrap_references(s22,4)
				), 
				op8_impl1{}
			)
		).get()) == &s22
	);
	BOOST_CHECK(
		&(wrap_reference(
			hana::unpack(
				unwrap_references(
					hana::make_tuple(evaluate(wrap_reference(unwrap_reference(wrap_reference(s22)))), evaluate(wrap_reference(4)))
				), 
				op8_impl1{}
			)
		).get()) == &s22
	);
	
	BOOST_CHECK(
		&(wrap_reference(
			hana::unpack(
				unwrap_references(
					hana::make_tuple(evaluate(wrap_reference(unwrap_reference(wrap_reference(s22)))), evaluate(wrap_reference(4)))
				), 
				op8_impl1{}
			)
		).get()) == &s22
	);
	
	//TODO: Rework for new evaluate
// 	BOOST_CHECK(hana::at_c<1>(op8(s22, 4).operands()) == 4);
// 	BOOST_CHECK(evaluate(hana::at_c<1>(op8(s22, 4).operands())) == 4);
// 	
// 	static_assert(std::is_same<
// 		decltype(evaluate_impl<expression_tag,no_context_tag>::apply(invoke(op8, s22, 4), make_no_context())),
// 		std::vector<int> &
// 	>{}, "");
	
	
	
	
	
	
	
	static_assert(std::is_same<
		decltype(evaluate(wrap_reference(4))),
		detail::rvalue_reference_wrapper< int >
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate(wrap_reference(hana::int_<1337>{}))),
		detail::rvalue_reference_wrapper< hana::int_<1337> >
	>{}, "");
	
	
	
	
	
	std::vector<int> & s44 = 
		hana::unpack(
			unwrap_references(wrap_references(s22, 4)),
			op8_impl1{}
		);
	
	
	hana::tuple<op8_impl1, detail::lvalue_reference_wrapper<std::vector<int>> , int> s45 = hana::unpack(
		wrap_references(s22, 4),
		hana::partial(hana::make_tuple, op8_impl1{})
	);
	
	
	static_assert(decltype(
		hana::unpack(
			unwrap_references(
				hana::unpack(
					wrap_references(s22, 4),
					hana::partial(hana::make_tuple, op8_impl1{})
				)
			),
			is_invocable
		)
		){}, "");

	static_assert(decltype(
		hana::compose(
			hana::reverse_partial(hana::unpack, is_invocable),
			hana::compose(
				unwrap_references, 
				hana::unpack(
					wrap_references(s22,4),
					hana::partial(hana::reverse_partial, hana::make_tuple)
				)
			)
		)(op8_impl1{})
	){}, "");
	
	
	static_assert(std::is_same<
		decltype(wrap_references(s22, 4)),
		hana::tuple< 
			detail::lvalue_reference_wrapper< std::vector<int> >,
			detail::rvalue_reference_wrapper< int >
		>
	>{}, "");
	
	hana::tuple<op8_impl1> s26 = detail::filter_invokables(op8, implementations_of(op8), hana::make_tuple(detail::type_of(s22), detail::type_of(4)), make_no_context());
	
	op8_impl1{}(s22, 4);
	
	std::vector<int> & s56 = evaluate(invoke(op8, s22, 4));
	
	std::vector<int> & s67 = evaluate(forward(s22));
	std::vector<int> & s68 = evaluate(forward(invoke(op8, s22, 4)));
	
	int s70 = 4;
	std::vector<int> & s71 = evaluate(forward(invoke(op8, s22, s70)));
	
	std::vector<int> & s69 = op12_impl1{}(s22, 4);
	
	
	
	hana::tuple<
		detail::lvalue_reference_wrapper< std::vector<int> >, 
		detail::rvalue_reference_wrapper<int>
	> s73 = wrap_references(s22, 4);
	
	std::vector<int> & s72 = hana::unpack(
		unwrap_references(wrap_references(s22, 4)),
		op12_impl1{}
	);
	
	hana::true_ s74 = hana::unpack(
		unwrap_references(hana::make_tuple(op12_impl1{}, detail::make_lvalue_reference_wrapper(s22), detail::make_rvalue_reference_wrapper(4))),
		is_invocable
	);
	
	
	
	static_assert(std::is_same<
		decltype(evaluate(op12(s22,4))),
		std::vector<int> &
	>{}, "");
	
	//TODO: Rework for new evaluate error handling and stack traces
// 	static_assert(std::is_same<
// 		decltype(evaluate(op12(s22,s70)).error().error().cause()),
// 		error_cause::not_implemented &&
// 	>{}, "");
	
	std::vector<t6> s78;
	static_assert(std::is_same<
		decltype(evaluate(op12(s78,t6{}))),
		std::vector<t6> &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate(invoke(op8, s22, 4))),
		std::vector<int> &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate(invoke(op8, s22, 4))),
		std::vector<int> &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate(unwrap_reference(wrap_reference(s22)), make_no_context())),
		std::vector<int> &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(detail::invoke_evaluate(op8, implementations_of(op8), wrap_references(s22, 4), make_no_context(), op8_impl1{})),
		std::vector<int> &
	>{}, "");
	
	BOOST_CHECK(
		&(detail::invoke_evaluate(
			op8, 
			implementations_of(op8), 
			wrap_references(s22, 4), 
			make_no_context(), 
			op8_impl1{})
		) == &s22
	);
	
	decltype(auto) s57 = hana::transform(
		wrap_references(s22),
		hana::compose(
			wrap_reference,
			hana::reverse_partial(
				evaluate,
				make_no_context()
			)
		)
	);
	
	static_assert(std::is_same<
		decltype(s57),
		hana::tuple< detail::lvalue_reference_wrapper < std::vector<int> > >
	>{}, "");
	
	static_assert(std::is_same<
		decltype(HBRS_MPL_FWD(s57)),
		hana::tuple< detail::lvalue_reference_wrapper < std::vector<int> > > &&
	>{}, "");
	
	static_assert(std::is_same<
		decltype(
			hana::transform(
				wrap_references(s22),
				hana::compose(
					wrap_reference,
					hana::reverse_partial(
						evaluate,
						make_no_context()
					)
				)
			)
		),
		hana::tuple< detail::lvalue_reference_wrapper < std::vector<int> > >
	>{}, "");
	
	static_assert(std::is_same<
		decltype(hana::id(unwrap_reference(evaluate(invoke(op8, s22, 4))))),
		std::vector<int> &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(hana::id(evaluate(invoke(op8, s22, 4)))),
		std::vector<int> &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(evaluate_impl<expression_tag,no_context_tag>::apply(invoke(op8, s22, 4), make_no_context())),
		std::vector<int> &
	>{}, "");
	
	static_assert(std::is_same<
		decltype(unwrap_reference(evaluate_impl<expression_tag,no_context_tag>::apply(invoke(op8, s22, 4), make_no_context()))),
		std::vector<int> &
	>{}, "");
	
	
	BOOST_CHECK(& (unwrap_reference(evaluate(invoke(op8, s22, 4)))) == &s22);
	BOOST_CHECK(& (evaluate(invoke(op8, s22, 4))) == &s22);
	
	
// 	static_assert(std::is_same<
// 		decltype(detail::evaluate_phase2(
// 			op9, 
// 			wrap_references(s62, 4),
// 			make_no_context(), 
// 			hana::tag_of_t<op9_impl1>{}
// 		)),
// 		std::vector<int>
// 	>{}, "");
	
	
	
	
	std::vector<int> s23 = evaluate(op8(s22,4));
	std::vector<int> s24 = op8_impl1{}(s22,4);
	
	std::vector<int> s46{1,2,3};
	std::vector<int> s47 = evaluate(op8(s46,4));
	
	BOOST_CHECK(s46.size() == 4);
	BOOST_CHECK(s46.at(3) == 4);
	
	BOOST_CHECK(s47.size() == 4);
	BOOST_CHECK(s47.at(3) == 4);
	
	BOOST_CHECK(s46.at(3) == s47.at(3));
	
	std::vector<int> const s48{1,2,3};
	evaluate(op9(s48,4));
	
	std::vector<int> s49{ evaluate(op9(s48,4)) };
	
	static_assert(std::is_same<
		decltype(evaluate(invoke(op9, s48, 4))),
		std::vector<int>
	>{}, "");
	
	BOOST_CHECK(s48.size() == 3);
	BOOST_CHECK(s49.size() == 4);
	BOOST_CHECK(s49.at(3) == 4);
	
	std::vector<int> s50{1,2,3};
	static_assert(std::is_same<
		hana::tag_of_t<decltype(evaluate(op9(s50,4)))>,
		error_tag
	>{}, "");
	
	static constexpr hana::true_ s84 = is_invocable(op14_impl1{});
	static constexpr hana::false_ s85 = is_invocable(op14_impl1{}, 1);
	static constexpr hana::true_ s86 = is_invocable(op14_impl2{}, 1);
	static constexpr hana::false_ s87 = is_invocable(op14_impl2{}, std::array<int,1>{});
	static constexpr hana::true_ s88 = is_invocable(op14_impl3{}, hana::size_c<1337>);
	static constexpr hana::true_ s89 = is_invocable(op14_impl4);
	static constexpr hana::false_ s90 = is_invocable(op14_impl4, std::array<int,1>{});
	static constexpr hana::true_ s91 = is_invocable(op14_impl5, 1);
	static constexpr hana::false_ s92 = is_invocable(op14_impl5, std::array<int,1>{});
	
	asm("nop");
	std::vector<int> s51{1,2,3};
	asm("nop");
	std::vector<int> s52{1,2,3};
	asm("nop");
	evaluate(op8(s51,4));
	asm("nop");
	op10(s52,4);
	asm("nop");
	asm("nop");
	evaluate(op8(s51,4));
	asm("nop");
	op10(s52,4);
	asm("nop");
	unwrap_reference(evaluate(op8(s51,4)));
	asm("nop");
	op10(s52,4);
	asm("nop");
	id(unwrap_reference(evaluate(op8(s51,4))));
	asm("nop");
	op10(s52,4);
	asm("nop");
	op11{}(detail::make_lvalue_reference_wrapper(s52).get(),4);
	asm("nop");
	asm("nop");
	asm("nop");
	auto s53 = unwrap_reference(evaluate(op8(s51,4)));
	asm("nop");
	auto s54 = op10(s52,4);
	asm("nop");
	
	asm("nopw 0x000000");
	int s82 = 0x1;
	asm("nopw 0x000001");
	//TODO: Fix this!
	//int s83 = evaluate(op13(0x1337));
	asm("nopw 0x000002");
	//BOOST_CHECK(evaluate(op13(0x1337)) == 2);
	asm("nopw 0x000003");
	
	//TODO:
// 	static_assert(std::is_same<
// 		typename decltype(detail::signature(op13_impl1{}, 0x1337))::type,
// 		std::function<int(int&&)>
// 	>{}, "");
	std::function<int(int&&)> s79 = op13_impl1{};
	BOOST_CHECK(s79(0x1337) == 1);
	
	std::function<int(int&&)> s80 = [](int&&){ return 1; }; 
	std::function<int(int&&)> s81 = [](int){ return 1; }; 
	
	constexpr decltype(auto) s93 = make_expression(1, wrap_references(hana::int_<1337>{}));
	constexpr auto s94 = make_expression(1, wrap_references(hana::int_<1337>{}));
	
	static_assert(std::is_same<
		decltype(s94),
		const expression<
			int, 
			hana::tuple<
				detail::rvalue_reference_wrapper<hana::int_<1337> > 
			> 
		>
	>{}, "");
	
	//NOTE: copy construction not allowed?!
	//constexpr auto s95 = s94;
	
	constexpr auto s96 = make_expression(666, wrap_references(hana::int_c<42>, hana::int_<1337>{}));
	
	static_assert(std::is_same<
		decltype(s96),
		const expression<
			int, 
			hana::tuple<
				detail::lvalue_reference_wrapper<hana::int_<42> const >,
				detail::rvalue_reference_wrapper<hana::int_<1337> > 
			> 
		>
	>{}, "");
	
	static_assert(evaluate(op15(hana::int_c<1337>)) == hana::int_c<1337>, "");
	
	//TODO: Fix those bugs:
	BOOST_CHECK(( evaluate(op15(std::vector<int>{1, 2, 3})).empty() == false ));
	BOOST_CHECK(( op15_impl{}(std::vector<int>{1, 2, 3}) .empty() == false ));
	
	BOOST_CHECK(( evaluate(op15(std::vector<int>{1, 2, 3})) == std::vector<int>{1, 2, 3} ));
	BOOST_CHECK(evaluate(op15(t7{})).valid);
	
	t7 s97{};
	t7 s98 = std::move(s97);
	t7 s99 = std::move(s97);
	
	BOOST_CHECK(  s98.valid );
	BOOST_CHECK( !s99.valid );
	
// 	constexpr t5 s100 = evaluate(forward(t5{}));
// 	constexpr t5 s101 = detail::forward_impl1{}(t5{});
	
	BOOST_TEST(
		unwrap_reference(
			[](auto && o) {
				auto _ = unwrap_reference(HBRS_MPL_FWD(o));
				return HBRS_MPL_FWD(o);
			}(
				wrap_reference(std::vector<int>{1,2,3})
			)
		).empty() == true
	);
	
	BOOST_TEST(
		unwrap_reference(
			[](auto && o) {
				auto _ = detail::type_of(unwrap_reference(HBRS_MPL_FWD(o)));
				return HBRS_MPL_FWD(o);
			}(
				wrap_reference(std::vector<int>{1,2,3})
			)
		).empty() == false
	);
	
	BOOST_TEST(
		unwrap_reference(
			[](auto && os) {
				auto _ = hana::transform(
					HBRS_MPL_FWD(os), hana::demux(detail::type_of)(unwrap_reference)
				);
				
				return hana::at_c<0>(HBRS_MPL_FWD(os));
			}(
				hana::make_tuple(wrap_reference(std::vector<int>{1,2,3}))
			)
		).empty() == false
	);
	
	BOOST_TEST(
		unwrap_reference(
			[](auto && os) {
				auto _ = detail::transform_and_forward(
					HBRS_MPL_FWD(os), hana::demux(detail::as_const_reference)(unwrap_reference)
				);
				
				return hana::at_c<0>(HBRS_MPL_FWD(os));
			}(
				hana::make_tuple(wrap_reference(std::vector<int>{1,2,3}))
			)
		).empty() == false
	);
	
	
	
	auto f = [](auto v) {
		v.clear();
	};
	std::vector<int> s102{1,2,3};
	f(s102);
	BOOST_TEST(s102.empty() == false);
	
	BOOST_TEST(
		[&f](auto && v) {
			f(v);
			return static_cast<decltype(v)&&>(v);
		}(std::vector<int>{1,2,3}).empty() == true
	);
	
	hana::int_<5> s103 = evaluate(id(hana::int_c<2>) + id(hana::int_c<3>));
}

BOOST_AUTO_TEST_SUITE_END()
