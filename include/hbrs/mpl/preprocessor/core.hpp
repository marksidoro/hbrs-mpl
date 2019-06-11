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

#ifndef HBRS_MPL_PP_CORE_HPP
#define HBRS_MPL_PP_CORE_HPP

/* Ref.: https://stackoverflow.com/a/11763277/6490710 */
#define HBRS_MPL_MACRO_2ARGS(_1,_2,NAME /*, ... */) NAME
/* NOTE: When using '...' gcc throws pedantic errors: 
 *  'error: ISO C++11 requires at least one argument for the "..." in a variadic macro'
 */

#define HBRS_MPL_FORWARD2(v_name, v_type)                                                                              \
	static_cast<v_type&&>(v_name) /* std::forward<v_type>(v_name) */

#define HBRS_MPL_FORWARD1(v_name)                                                                                      \
	HBRS_MPL_FORWARD2(v_name, decltype(v_name))

#define HBRS_MPL_FORWARD(...)                                                                                          \
	HBRS_MPL_MACRO_2ARGS(__VA_ARGS__, HBRS_MPL_FORWARD2, HBRS_MPL_FORWARD1)(__VA_ARGS__)

#define HBRS_MPL_FWD(...)                                                                                              \
	HBRS_MPL_FORWARD(__VA_ARGS__)

#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/basic_tuple.hpp>

#define HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY1(f_name)                                                                  \
	template <                                                                                                         \
		typename T1,                                                                                                   \
		typename = void                                                                                                \
	>                                                                                                                  \
	struct f_name ## _impl : f_name ## _impl<T1, boost::hana::when<true>> { };                                         \
                                                                                                                       \
	struct f_name ## _t {                                                                                              \
		template <                                                                                                     \
			typename T1                                                                                                \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T1&&) const;                                                                                        \
	};                                                                                                                 \
                                                                                                                       \
	inline constexpr f_name ## _t f_name{};

#define HBRS_MPL_DEC_CF1(...)                                                                                          \
	HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY1(__VA_ARGS__)

#define HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY1(f_name)                                                                   \
    template <                                                                                                         \
		typename T1                                                                                                    \
	>                                                                                                                  \
    constexpr decltype(auto)                                                                                           \
	f_name ## _t::operator()(T1 && t1) const {                                                                         \
        return f_name ## _impl<                                                                                        \
			boost::hana::tag_of_t<T1>                                                                                  \
		>::apply( static_cast<T1&&>(t1) );                                                                             \
    }

#define HBRS_MPL_DEF_CF1(...)                                                                                          \
	HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY1(__VA_ARGS__)

#define HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY2(f_name)                                                                  \
	template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2,                                                                                                   \
		typename = void                                                                                                \
	>                                                                                                                  \
	struct f_name ## _impl : f_name ## _impl<T1, T2, boost::hana::when<true>> { };                                     \
                                                                                                                       \
	struct f_name ## _t {                                                                                              \
		template <                                                                                                     \
			typename T1,                                                                                               \
			typename T2                                                                                                \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T1&&, T2&&) const;                                                                                  \
	};                                                                                                                 \
                                                                                                                       \
	inline constexpr f_name ## _t f_name{};

#define HBRS_MPL_DEC_CF2(...)                                                                                          \
	HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY2(__VA_ARGS__)
	
#define HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY2(f_name)                                                                   \
    template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2                                                                                                    \
	>                                                                                                                  \
    constexpr decltype(auto)                                                                                           \
	f_name ## _t::operator()(T1 && t1, T2 && t2) const {                                                               \
        return f_name ## _impl<                                                                                        \
			boost::hana::tag_of_t<T1>,                                                                                 \
			boost::hana::tag_of_t<T2>                                                                                  \
		>::apply( static_cast<T1&&>(t1), static_cast<T2&&>(t2) );                                                      \
    }

#define HBRS_MPL_DEF_CF2(...)                                                                                          \
	HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY2(__VA_ARGS__)
	
#define HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY3(f_name)                                                                  \
	template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2,                                                                                                   \
		typename T3,                                                                                                   \
		typename = void                                                                                                \
	>                                                                                                                  \
	struct f_name ## _impl : f_name ## _impl<T1, T2, T3, boost::hana::when<true>> { };                                 \
                                                                                                                       \
	struct f_name ## _t {                                                                                              \
		template <                                                                                                     \
			typename T1,                                                                                               \
			typename T2,                                                                                               \
			typename T3                                                                                                \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T1&&, T2&&, T3&&) const;                                                                            \
	};                                                                                                                 \
                                                                                                                       \
	inline constexpr f_name ## _t f_name{};

#define HBRS_MPL_DEC_CF3(...)                                                                                          \
	HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY3(__VA_ARGS__)
	
#define HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY3(f_name)                                                                   \
    template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2,                                                                                                   \
		typename T3                                                                                                    \
	>                                                                                                                  \
    constexpr decltype(auto)                                                                                           \
	f_name ## _t::operator()(T1 && t1, T2 && t2, T3 && t3) const {                                                     \
        return f_name ## _impl<                                                                                        \
			boost::hana::tag_of_t<T1>,                                                                                 \
			boost::hana::tag_of_t<T2>,                                                                                 \
			boost::hana::tag_of_t<T3>                                                                                  \
		>::apply( static_cast<T1&&>(t1), static_cast<T2&&>(t2), static_cast<T3&&>(t3) );                               \
    }

#define HBRS_MPL_DEF_CF3(...)                                                                                          \
	HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY3(__VA_ARGS__)


#define HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY4(f_name)                                                                  \
	template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2,                                                                                                   \
		typename T3,                                                                                                   \
		typename T4,                                                                                                   \
		typename = void                                                                                                \
	>                                                                                                                  \
	struct f_name ## _impl : f_name ## _impl<T1, T2, T3, T4, boost::hana::when<true>> { };                             \
                                                                                                                       \
	struct f_name ## _t {                                                                                              \
		template <                                                                                                     \
			typename T1,                                                                                               \
			typename T2,                                                                                               \
			typename T3,                                                                                               \
			typename T4                                                                                                \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T1&&, T2&&, T3&&, T4&&) const;                                                                      \
	};                                                                                                                 \
                                                                                                                       \
	inline constexpr f_name ## _t f_name{};

#define HBRS_MPL_DEC_CF4(...)                                                                                          \
	HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY4(__VA_ARGS__)
	
#define HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY4(f_name)                                                                   \
    template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2,                                                                                                   \
		typename T3,                                                                                                   \
		typename T4                                                                                                    \
	>                                                                                                                  \
    constexpr decltype(auto)                                                                                           \
	f_name ## _t::operator()(T1 && t1, T2 && t2, T3 && t3, T4 && t4) const {                                           \
        return f_name ## _impl<                                                                                        \
			boost::hana::tag_of_t<T1>,                                                                                 \
			boost::hana::tag_of_t<T2>,                                                                                 \
			boost::hana::tag_of_t<T3>,                                                                                 \
			boost::hana::tag_of_t<T4>                                                                                  \
		>::apply(                                                                                                      \
			static_cast<T1&&>(t1),                                                                                     \
			static_cast<T2&&>(t2),                                                                                     \
			static_cast<T3&&>(t3),                                                                                     \
			static_cast<T4&&>(t4)                                                                                      \
		);                                                                                                             \
    }

#define HBRS_MPL_DEF_CF4(...)                                                                                          \
	HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY4(__VA_ARGS__)

#define HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY5(f_name)                                                                  \
	template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2,                                                                                                   \
		typename T3,                                                                                                   \
		typename T4,                                                                                                   \
		typename T5,                                                                                                   \
		typename = void                                                                                                \
	>                                                                                                                  \
	struct f_name ## _impl : f_name ## _impl<T1, T2, T3, T4, T5, boost::hana::when<true>> { };                         \
                                                                                                                       \
	struct f_name ## _t {                                                                                              \
		template <                                                                                                     \
			typename T1,                                                                                               \
			typename T2,                                                                                               \
			typename T3,                                                                                               \
			typename T4,                                                                                               \
			typename T5                                                                                                \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(T1&&, T2&&, T3&&, T4&&, T5&&) const;                                                                \
	};                                                                                                                 \
                                                                                                                       \
	inline constexpr f_name ## _t f_name{};

#define HBRS_MPL_DEC_CF5(...)                                                                                          \
	HBRS_MPL_DECLARE_CORE_FUNCTION_ARITY5(__VA_ARGS__)
	
#define HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY5(f_name)                                                                   \
    template <                                                                                                         \
		typename T1,                                                                                                   \
		typename T2,                                                                                                   \
		typename T3,                                                                                                   \
		typename T4,                                                                                                   \
		typename T5                                                                                                    \
	>                                                                                                                  \
    constexpr decltype(auto)                                                                                           \
	f_name ## _t::operator()(T1 && t1, T2 && t2, T3 && t3, T4 && t4, T5 && t5) const {                                 \
        return f_name ## _impl<                                                                                        \
			boost::hana::tag_of_t<T1>,                                                                                 \
			boost::hana::tag_of_t<T2>,                                                                                 \
			boost::hana::tag_of_t<T3>,                                                                                 \
			boost::hana::tag_of_t<T4>,                                                                                 \
			boost::hana::tag_of_t<T5>                                                                                  \
		>::apply(                                                                                                      \
			static_cast<T1&&>(t1),                                                                                     \
			static_cast<T2&&>(t2),                                                                                     \
			static_cast<T3&&>(t3),                                                                                     \
			static_cast<T4&&>(t4),                                                                                     \
			static_cast<T5&&>(t5)                                                                                      \
		);                                                                                                             \
    }

#define HBRS_MPL_DEF_CF5(...)                                                                                          \
	HBRS_MPL_DEFINE_CORE_FUNCTION_ARITY5(__VA_ARGS__)

#define HBRS_MPL_DECLARE_CORE_FUNCTION_VARIADIC(f_name)                                                                \
	template <                                                                                                         \
		typename T,                                                                                                    \
		typename = void                                                                                                \
	>                                                                                                                  \
	struct f_name ## _impl : f_name ## _impl<T, boost::hana::when<true>> { };                                          \
                                                                                                                       \
	struct f_name ## _t {                                                                                              \
		template <                                                                                                     \
			typename... Ts                                                                                             \
		>                                                                                                              \
		constexpr decltype(auto)                                                                                       \
		operator()(Ts&&...) const;                                                                                     \
	};                                                                                                                 \
                                                                                                                       \
	inline constexpr f_name ## _t f_name{};

#define HBRS_MPL_DEC_CFV(...)                                                                                          \
	HBRS_MPL_DECLARE_CORE_FUNCTION_VARIADIC(__VA_ARGS__)

#define HBRS_MPL_DEFINE_CORE_FUNCTION_VARIADIC(f_name)                                                                 \
    template <                                                                                                         \
		typename... Ts                                                                                                 \
	>                                                                                                                  \
    constexpr decltype(auto)                                                                                           \
	f_name ## _t::operator()(Ts&&...ts) const {                                                                        \
        return f_name ## _impl<                                                                                        \
			boost::hana::basic_tuple<boost::hana::tag_of_t<Ts>...>                                                     \
		>::apply( static_cast<Ts&&>(ts)... );                                                                          \
    }

#define HBRS_MPL_DEF_CFV(...)                                                                                          \
	HBRS_MPL_DEFINE_CORE_FUNCTION_VARIADIC(__VA_ARGS__)

#include <boost/hana/equal.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/integral_constant.hpp>

#define HBRS_MPL_DEFINE_FLAG(t_name)                                                                                   \
	struct t_name {                                                                                                    \
		constexpr boost::hana::true_                                                                                   \
		operator==(t_name const&) const { return {}; }                                                                 \
		                                                                                                               \
		constexpr boost::hana::false_                                                                                  \
		operator!=(t_name const&) const { return {}; }                                                                 \
	};

#define HBRS_MPL_DEF_FLAG(...)                                                                                         \
	HBRS_MPL_DEFINE_FLAG(__VA_ARGS__)


#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/for_each_product.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/to_tuple.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/tuple/push_front.hpp>
#include <boost/preprocessor/seq/cat.hpp>

#define _CV_REF_PRODUCT(r, product)                                                                                    \
	(BOOST_PP_SEQ_TO_TUPLE(product))

#define _FORMAT_TYPE_CV_REF(t, cv_ref)                                                                                 \
    (BOOST_PP_TUPLE_PUSH_FRONT(cv_ref, t))

#define _TYPE_CV_REF_PRODUCT(r, elem)                                                                                  \
    BOOST_PP_EXPAND(_FORMAT_TYPE_CV_REF BOOST_PP_SEQ_TO_TUPLE(elem))

#define _QUALIFIED_TYPES_SEQ_PRODUCT(r, data, elem)                                                                    \
    ( BOOST_PP_SEQ_FOR_EACH_PRODUCT(_TYPE_CV_REF_PRODUCT, ((elem))(data)) )

#define _QUALIFIED_TYPE_TUPLE_TO_SEQ(r, data, elem)                                                                    \
    BOOST_PP_TUPLE_TO_SEQ(elem)

#define _CONCAT_QUALIFIED_TYPES_TUPLES(...)                                                                            \
    BOOST_PP_SEQ_FOR_EACH(_QUALIFIED_TYPE_TUPLE_TO_SEQ, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define _QUALIFIED_TYPES_TUPLE_PRODUCT(r, product)                                                                      \
    (BOOST_PP_SEQ_TO_TUPLE( BOOST_PP_EXPAND(_CONCAT_QUALIFIED_TYPES_TUPLES BOOST_PP_SEQ_TO_TUPLE(product)) ))

#define _APPLY_F(r, F, declarations)                                                                                    \
    BOOST_PP_EXPAND(F declarations)

#define HBRS_MPL_INSTANTIATE(F, type_specifiers, cv_qualifiers, ref_declarators)                                       \
    BOOST_PP_SEQ_FOR_EACH(_APPLY_F, F,                                                                                 \
        BOOST_PP_SEQ_FOR_EACH_PRODUCT(_QUALIFIED_TYPES_TUPLE_PRODUCT,                                                  \
            BOOST_PP_SEQ_FOR_EACH(                                                                                     \
                _QUALIFIED_TYPES_SEQ_PRODUCT,                                                                          \
                BOOST_PP_SEQ_FOR_EACH_PRODUCT(_CV_REF_PRODUCT, (cv_qualifiers)(ref_declarators) ),                     \
                type_specifiers                                                                                        \
            )                                                                                                          \
        )                                                                                                              \
    )

#define HBRS_MPL_INSTANTIATE_FORWARDING_REFERENCES(F, ... /* types */)                                                 \
	HBRS_MPL_INSTANTIATE(                                                                                              \
		F,                                                                                                             \
		BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__),                                                                         \
		/* Ref.: https://en.cppreference.com/w/cpp/language/declarations */                                            \
		/* TODO: volatile? const volatile? mutable? */                                                                 \
		()(const),                                                                                                     \
		(&)(&&)                                                                                                        \
	)

#define HBRS_MPL_INST_FWDREFS(...)                                                                                     \
	HBRS_MPL_INSTANTIATE_FORWARDING_REFERENCES(__VA_ARGS__)

/* Example inputs (BOOST_...) and outputs (#define _EXAMPLE...) for above macros:
 * 
 * BOOST_PP_SEQ_FOR_EACH_PRODUCT(_CV_REF_PRODUCT, (()(const)) ((&)(&&)) )
 * #define _EXAMPLE1 ((, &)) ((, &&)) ((const, &)) ((const, &&))
 * BOOST_PP_SEQ_FOR_EACH(_QUALIFIED_TYPES_SEQ_PRODUCT, _EXAMPLE1 , (float)(int))
 * #define _EXAMPLE2 ( ((float , , &)) ((float , , &&)) ((float , const, &)) ((float , const, &&)) ) ( ((int , , &)) ((int , , &&)) ((int , const, &)) ((int , const, &&)) )
 * BOOST_PP_SEQ_FOR_EACH_PRODUCT(_QUALIFIED_TYPES_TUPLE_PRODUCT, _EXAMPLE2)
 * #define _EXAMPLE3 ((float, , &, int, , &)) ((float, , &, int, , &&)) ((float, , &, int, const, &)) ((float, , &, int, const, &&)) ((float, , &&, int, , &)) ((float, , &&, int, , &&)) ((float, , &&, int, const, &)) ((float, , &&, int, const, &&)) ((float, const, &, int, , &)) ((float, const, &, int, , &&)) ((float, const, &, int, const, &)) ((float, const, &, int, const, &&)) ((float, const, &&, int, , &)) ((float, const, &&, int, , &&)) ((float, const, &&, int, const, &)) ((float, const, &&, int, const, &&))
 * #define _EXAMPLE4(ts1, cvq1, rd1, ts2, cvq2, rd2)  f(ts1 cvq1 rd1, ts2 cvq2 rd2);
 * BOOST_PP_SEQ_FOR_EACH(_APPLY_F, _EXAMPLE4, _EXAMPLE3)
 * #define _EXAMPLE5 f(float &, int &); f(float &, int &&); f(float &, int const &); f(float &, int const &&); f(float &&, int &); f(float &&, int &&); f(float &&, int const &); f(float &&, int const &&); f(float const &, int &); f(float const &, int &&); f(float const &, int const &); f(float const &, int const &&); f(float const &&, int &); f(float const &&, int &&); f(float const &&, int const &); f(float const &&, int const &&);
 * HBRS_MPL_INST_FWDREFS(_EXAMPLE4, float, int)
 * #define _EXAMPLE6 _EXAMPLE5
 */


#endif // !HBRS_MPL_PP_CORE_HPP
