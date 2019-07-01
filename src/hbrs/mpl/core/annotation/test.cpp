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

#define BOOST_TEST_MODULE annotation_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/core/annotation.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/integral_constant.hpp>

struct t1{
	constexpr boost::hana::true_
	operator==(t1 const&) const { return {}; }

	constexpr boost::hana::false_
	operator!=(t1 const&) const { return {}; }
};
struct t2{
	constexpr boost::hana::true_
	operator==(t2 const&) const { return {}; }

	constexpr boost::hana::false_
	operator!=(t2 const&) const { return {}; }
};


BOOST_AUTO_TEST_SUITE(annotation_test)

BOOST_AUTO_TEST_CASE(annotation_test_1) {
	using namespace hbrs::mpl;
	
	
	using detail::make_annotated;
	using detail::annotated;
	
	t1 s37;
	t1 const s38;
	
	static_assert(std::is_same<
		decltype(object(make_annotated(s37, hana::make_tuple(t2{})))),
		t1
	>{}, "");
	
	static_assert(std::is_same<
		decltype((s38)),
		t1 const&
	>{}, "");
	
	static_assert(std::is_same<
		decltype(object(make_annotated(s38, hana::make_tuple(t2{})))),
		t1
	>{}, "");
	
	static_assert(std::is_same<
		decltype(object(make_annotated(t1{}, hana::make_tuple(t2{})))),
		t1
	>{}, "");
	
	constexpr annotated<t1, hana::tuple<t2>> s4 = make_annotated(t1{}, hana::make_tuple(t2{}));
	constexpr annotated<t1, hana::tuple<t2, t2>> s5 = add_annotation(s4, t2{});
	constexpr annotated<t1, hana::tuple<t1, t2>> s28 = add_annotation(add_annotation(t1{}, t1{}), t2{});
	hana::true_ s29 = has_annotations(s28);
	hana::false_ s30 = has_annotations(make_annotated(t1{}, hana::make_tuple() ));
	
	constexpr t1 s6 = object(make_annotated(t1{}, t2{}));
	static_assert(has_annotation(add_annotation(t1{}, t2{}), t1{}) == hana::false_c, "");
	static_assert(has_annotation(add_annotation(t1{}, t2{}), t2{}) == hana::true_c, "");
	
	static_assert(hana::equal(t2{}, t1{}) == hana::false_c, "");
	
	static_assert(has_annotation(add_annotation(t1{}, t2{}), t2{}) == hana::true_c, "");
	static_assert(
		has_annotation(
			remove_annotation(
				add_annotation(t1{}, t2{}), 
				t2{}
			), 
		t2{}) == hana::false_c, "");
	
	static_assert(
		has_annotation(
			remove_annotation(
				add_annotation(add_annotation(t1{}, t1{}), t2{}), 
				t2{}
			), 
		t1{}) == hana::true_c, "");
	
	
}

BOOST_AUTO_TEST_SUITE_END()
