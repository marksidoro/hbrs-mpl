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

#ifndef HBRS_MPL_CORE_ANNOTATION_IMPL_HPP
#define HBRS_MPL_CORE_ANNOTATION_IMPL_HPP

#include "fwd.hpp"
#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

namespace detail {

template<typename Object, typename Annotations>
struct annotated {
	
	template<
		typename Object_, 
		typename Annotations_
	>
	constexpr 
	annotated(Object_&& o, Annotations_&& as) 
	: object_{HBRS_MPL_FWD(o)}, annotations_{HBRS_MPL_FWD(as)} {}
	
	constexpr 
	annotated() : object_{}, annotations_{} {}
	
	constexpr 
	annotated(annotated const&) = default;
	
	constexpr 
	annotated(annotated &&) = default;
	
	constexpr decltype(auto)
	object() & { return (object_); }
	
	constexpr decltype(auto)
	object() const& { return (object_); }
	
	constexpr decltype(auto)
	object() && { return HBRS_MPL_FWD(object_); }
	
	constexpr decltype(auto)
	annotations() & { return (annotations_); }
	
	constexpr decltype(auto)
	annotations() const& { return (annotations_); }
	
	constexpr decltype(auto)
	annotations() && { return HBRS_MPL_FWD(annotations_); }
	
private:
	Object object_;
	Annotations annotations_;
};
/* namespace detail */ }

HBRS_MPL_NAMESPACE_END



namespace boost { namespace hana {

template <typename Object, typename Annotations>
struct tag_of< hbrs::mpl::detail::annotated<Object, Annotations> > {
	using type = hbrs::mpl::detail::annotated_tag;
};

template <>
struct make_impl<hbrs::mpl::detail::annotated_tag> {
	
	template <typename Object, typename Annotations>
	static constexpr hbrs::mpl::detail::annotated<
		std::decay_t<Object>, 
		std::decay_t<Annotations>
	>
	apply(Object && o, Annotations&& as) {
		return {HBRS_MPL_FWD(o), HBRS_MPL_FWD(as)};
	}
	
};

/* namespace hana */ } /* namespace boost */ }



#include <hbrs/mpl/core/reference_wrapper/fwd.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template <>
struct wrap_reference_impl<detail::annotated_tag> {
	template <typename T>
	static constexpr auto
	apply(T && t) {
		return HBRS_MPL_FWD(t);
	}
};

HBRS_MPL_NAMESPACE_END

#include <boost/hana/remove.hpp>
#include <boost/hana/core/when.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/is_empty.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/append.hpp>
#include <boost/hana/integral_constant.hpp>

HBRS_MPL_NAMESPACE_BEGIN


HBRS_MPL_DEF_CF1(annotations)

template <>
struct annotations_impl<detail::annotated_tag>{
	template <
		typename Annotated
	>
	static constexpr decltype(auto)
	apply(Annotated && at) {
		return HBRS_MPL_FWD(at).annotations();
	}
};

template <typename ObjectTag, bool Condition>
struct annotations_impl<ObjectTag, hana::when<Condition>> : hana::default_ {
	
	template <
		typename Object
	>
	static constexpr auto
	apply(Object &&) {
		return hana::make_tuple();
	}
};

HBRS_MPL_DEF_CF1(object)

template <>
struct object_impl<detail::annotated_tag>{
	template <
		typename Annotated
	>
	static constexpr auto
	apply(Annotated && at) {
		return HBRS_MPL_FWD(at).object();
	}
};

template <typename ObjectTag, bool Condition>
struct object_impl<ObjectTag, hana::when<Condition>> : hana::default_ {
	
	template <
		typename Object
	>
	static constexpr auto
	apply(Object && o) {
		return HBRS_MPL_FWD(o);
	}
};

HBRS_MPL_DEF_CF1(has_annotations)

template <>
struct has_annotations_impl<detail::annotated_tag>{
	template <
		typename Annotated
	>
	static constexpr auto
	apply(Annotated && at) {
		return !hana::is_empty(HBRS_MPL_FWD(at).annotations());
	}
};

template <typename ObjectTag, bool Condition>
struct has_annotations_impl<ObjectTag, hana::when<Condition>> : hana::default_ {
	
	template <
		typename Object
	>
	static constexpr hana::false_
	apply(Object && o) {
		return hana::false_c;
	}
};


HBRS_MPL_DEF_CF2(has_annotation)

template <typename AnnotationTag>
struct has_annotation_impl<detail::annotated_tag, AnnotationTag>{
	template <
		typename Annotated,
		typename Annotation
	>
	static constexpr auto
	apply(Annotated && at, Annotation && a) {
		return hana::contains(
			HBRS_MPL_FWD(at).annotations(),
			HBRS_MPL_FWD(a)
		);
	}
};

template <typename ObjectTag, typename AnnotationTag, bool Condition>
struct has_annotation_impl<ObjectTag, AnnotationTag, hana::when<Condition>> : hana::default_ {
	
	template <
		typename Object,
		typename Annotation
	>
	static constexpr hana::false_
	apply(Object && o, Annotation && a) {
		return hana::false_c;
	}
};


HBRS_MPL_DEF_CF2(add_annotation)

template <typename AnnotationTag>
struct add_annotation_impl<detail::annotated_tag, AnnotationTag>{
	template <
		typename Annotated,
		typename Annotation
	>
	static constexpr auto
	apply(Annotated && at, Annotation && a) {
		return detail::make_annotated(
			object(HBRS_MPL_FWD(at)),
			hana::append(
				annotations(HBRS_MPL_FWD(at)),
				HBRS_MPL_FWD(a)
			)
		);
	}
};

template <typename ObjectTag, typename AnnotationTag, bool Condition>
struct add_annotation_impl<ObjectTag, AnnotationTag, hana::when<Condition>> : hana::default_ {
	
	template <
		typename Object,
		typename Annotation
	>
	static constexpr auto
	apply(Object && o, Annotation && a) {
		return detail::make_annotated(
			HBRS_MPL_FWD(o), 
			hana::make_tuple(HBRS_MPL_FWD(a))
		);
	}
};


HBRS_MPL_DEF_CF1(unwrap_if_no_annotations)

template <>
struct unwrap_if_no_annotations_impl<detail::annotated_tag>{
	template <
		typename Annotated,
		typename std::enable_if_t<
			decltype(has_annotations(std::declval<Annotated>()))::value
		>* = nullptr
	>
	static constexpr auto
	apply(Annotated && at) {
		return HBRS_MPL_FWD(at);
	}
	
	template <
		typename Annotated,
		typename std::enable_if_t<
			decltype(!has_annotations(std::declval<Annotated>()))::value
		>* = nullptr
	>
	static constexpr auto
	apply(Annotated && at) {
		return HBRS_MPL_FWD(at).object();
	}
	
};

template <typename ObjectTag, bool Condition>
struct unwrap_if_no_annotations_impl<ObjectTag, hana::when<Condition>> : hana::default_ {
	template <typename Object>
	static constexpr auto
	apply(Object && o) {
		return HBRS_MPL_FWD(o);
	}
};

HBRS_MPL_DEF_CF2(remove_annotation)

template <typename AnnotationTag>
struct remove_annotation_impl<detail::annotated_tag, AnnotationTag> {
	
	template <
		typename Annotated,
		typename Annotation
	>
	static constexpr auto
	apply(Annotated && at, Annotation && a) {
		return unwrap_if_no_annotations(
			detail::make_annotated(
				HBRS_MPL_FWD(at).object(),
				hana::remove(
					HBRS_MPL_FWD(at).annotations(),
					HBRS_MPL_FWD(a)
				)
			)
		);
	}

};

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_ANNOTATION_IMPL_HPP
