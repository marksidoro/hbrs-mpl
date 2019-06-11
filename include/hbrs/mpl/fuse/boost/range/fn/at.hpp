/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FUSE_BOOST_RANGE_FN_AT_HPP
#define HBRS_MPL_FUSE_BOOST_RANGE_FN_AT_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/size.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct at_impl_range_integer_range {
	template<
		typename Integer,
		typename std::enable_if_t<
			std::is_integral<Integer>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(boost::integer_range<Integer> s, Integer n) const {
		return s(n);
	}
	
	template<
		typename Integer,
		typename Integral,
		typename std::enable_if_t<
			std::is_integral<Integer>::value &&
			std::is_integral<Integral>::value &&
			!std::is_same<Integer, Integral>::value
		>* = nullptr
	>
	constexpr decltype(auto)
	operator()(boost::integer_range<Integer> s, Integral n) const {
		return s(boost::numeric_cast<Integer, Integral>(n));
	}
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FUSE_BOOST_RANGE_FN_AT_IMPLS boost::hana::make_tuple(                                                 \
		hbrs::mpl::detail::at_impl_range_integer_range{}                                                               \
	)

#endif // !HBRS_MPL_FUSE_BOOST_RANGE_FN_AT_HPP
