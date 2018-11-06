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

#pragma once

#ifndef MATLAB_FUSE_FN_SELECT_HPP
#define MATLAB_FUSE_FN_SELECT_HPP

#include <hbrs/mpl/preprocessor/core.hpp>

#include <matlab/dt/matrix.hpp>
#include <hbrs/mpl/dt/submatrix.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

struct select_impl_matrix {
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matlab::matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		mpl::range<
			mpl::matrix_index<int, int>,
			mpl::matrix_index<int, int>
		>  const& rng
	) const {
		using namespace hbrs::mpl;
		
		return operator()(
			HBRS_MPL_FWD(a),
			std::make_pair(
				rng.first(),
				make_matrix_size(
					rng.last().m() - rng.first().m() + 1,
					rng.last().n() - rng.first().n() + 1
				)
			)
		);
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matlab::matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a, 
		std::pair<mpl::range<int, int>, mpl::range<int,int>> const& rng
	) const {
		using namespace hbrs::mpl;
		
		auto first_m = hana::first(rng).first();
		auto last_m  = hana::first(rng).last();
		auto first_n = hana::second(rng).first();
		auto last_n = hana::second(rng).last();
		
		return operator()(
			HBRS_MPL_FWD(a),
			std::make_pair(
				make_matrix_index(first_m, first_n),
				make_matrix_size(
					last_m - first_m + 1,
					last_n - first_n + 1
				)
			)
		);
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matlab::matrix_tag >::value &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	auto
	operator()(
		Matrix && a,
		std::pair<
			mpl::matrix_index<int, int>,
			mpl::matrix_size<int, int>
		>  const& rng
	) const {
		using namespace hbrs::mpl;
		
		return submatrix<
			Matrix,
			matrix_index<int, int>,
			matrix_size<int, int>
		>{ HBRS_MPL_FWD(a), hana::first(rng), hana::second(rng) };
	}
	
	template<typename BaseType>
	auto
	operator()(
		matlab::matrix<BaseType> a,
		std::pair<
			mpl::matrix_index<int, int>,
			mpl::matrix_size<int, int>
		>  const& rng
	) const {
		using namespace hbrs::mpl;
		
		auto idx = hana::first(rng);
		auto sz = hana::second(rng);
		matlab::matrix<std::remove_const_t<BaseType>> b{sz.m(), sz.n()};
		
		for(int i = 0; i < sz.m(); ++i) {
			for(int j = 0; j < sz.n(); ++j) {
				(*at)(b, make_matrix_index(i,j)) = (*at)(a, make_matrix_index(idx.m()+i, idx.n()+j));
			}
		}
		
		return b;
	}
	
};

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_SELECT_IMPLS boost::hana::make_tuple(                                                           \
		matlab::detail::select_impl_matrix{}                                                                           \
	)

#endif // !MATLAB_FUSE_FN_SELECT_HPP
