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

#pragma once

#ifndef HBRS_MPL_TEST_DETAIL_HPP
#define HBRS_MPL_TEST_DETAIL_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <hbrs/mpl/detail/integral_value.hpp>
#include <cstdlib>

#define _BOOST_TEST_MMEQ(a, b, ucmp)                                                                                   \
	{                                                                                                                  \
		using namespace hbrs::mpl;                                                                                     \
		using hbrs::mpl::detail::integral_value;                                                                       \
		auto a_m = (*m)(size(a));                                                                                      \
		auto b_m = (*m)(size(b));                                                                                      \
		BOOST_TEST((*equal)(a_m, b_m), "a_m: " << a_m << " b_m: " << b_m);                                             \
		                                                                                                               \
		auto a_n = (*n)(size(a));                                                                                      \
		auto b_n = (*n)(size(b));                                                                                      \
		                                                                                                               \
		BOOST_TEST((*equal)(a_n, b_n), "a_n: " << a_n << " b_n: " << b_n);                                             \
		                                                                                                               \
		if ((*equal)(a_m, b_m) && (*equal)(a_n, b_n)) {                                                                \
			for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(a_m)); ++i) {                   \
				for(std::size_t j = 0; j < boost::numeric_cast<std::size_t>(integral_value(a_n)); ++j) {               \
					auto x = (*at)(a, make_matrix_index(i, j));                                                        \
					auto y = (*at)(b, make_matrix_index(i, j));                                                        \
					if (ucmp) {                                                                                        \
						x = std::abs(x);                                                                               \
						y = std::abs(y);                                                                               \
					}                                                                                                  \
					BOOST_TEST(x == y, "@[" << i << "][" << j << "] := Left: " << x << " Right: " << y);               \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
	}

#define _BOOST_TEST_VVEQ(a, b, ucmp)                                                                                   \
	{                                                                                                                  \
		using namespace hbrs::mpl;                                                                                     \
		using hbrs::mpl::detail::integral_value;                                                                       \
		auto a_sz = (*size)(a);                                                                                        \
		auto b_sz = (*size)(b);                                                                                        \
		BOOST_TEST((*equal)(a_sz, b_sz), "a_sz: " << a_sz << " b_sz: " << b_sz);                                       \
		                                                                                                               \
		if ((*equal)(a_sz, b_sz)) {                                                                                    \
			for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(a_sz)); ++i) {                  \
				auto x = (*at)(a, i);                                                                                  \
				auto y = (*at)(b, i);                                                                                  \
				if (ucmp) {                                                                                            \
					x = std::abs(x);                                                                                   \
					y = std::abs(y);                                                                                   \
				}                                                                                                      \
				BOOST_TEST(x == y, "@[" << i << "] := Left: " << x << " Right: " << y);                                \
			}                                                                                                          \
		}                                                                                                              \
	}

#define _BOOST_TEST_IS_IDENTITY_MATRIX(a)                                                                              \
	{                                                                                                                  \
		using namespace hbrs::mpl;                                                                                     \
		using hbrs::mpl::detail::integral_value;                                                                       \
		                                                                                                               \
		auto sz_ = (*size)(a);                                                                                         \
		auto m_ = (*m)(sz_);                                                                                           \
		auto n_ = (*n)(sz_);                                                                                           \
		for(std::size_t i = 0; i < boost::numeric_cast<std::size_t>(integral_value(m_)); ++i) {                        \
			for(std::size_t j = 0; j < boost::numeric_cast<std::size_t>(integral_value(n_)); ++j) {                    \
				auto x = (*at)(a, make_matrix_index(i, j));                                                            \
				if (i == j) {                                                                                          \
					BOOST_TEST(x == 1.0, "@[" << i << "][" << j << "] := " << x << " != 1");                           \
				} else {                                                                                               \
					BOOST_TEST(x == 0.0, "@[" << i << "][" << j << "] := " << x << " != 0");                           \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
	}

#endif // !HBRS_MPL_TEST_DETAIL_HPP
