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

#ifndef HBRS_MPL_FN_SELECT_FWD_MATLAB_HPP
#define HBRS_MPL_FN_SELECT_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/range/fwd.hpp>
	#include <hbrs/mpl/dt/matrix_index/fwd.hpp>
	#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

struct select_impl_ml_matrix {
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, ml_matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		range<
			matrix_index<int, int>,
			matrix_index<int, int>
		>  const& rng
	) const;
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, ml_matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a, 
		std::pair<range<int, int>, range<int,int>> const& rng
	) const;
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, ml_matrix_tag >::value &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	auto
	operator()(
		Matrix && a,
		std::pair<
			matrix_index<int, int>,
			matrix_size<int, int>
		>  const& rng
	) const;
	
	template<typename BaseType>
	auto
	operator()(
		ml_matrix<BaseType> a,
		std::pair<
			matrix_index<int, int>,
			matrix_size<int, int>
		>  const& rng
	) const;
};

#else
struct select_impl_ml_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SELECT_IMPLS_MATLAB boost::hana::make_tuple(                                                       \
		hbrs::mpl::detail::select_impl_ml_matrix{}                                                                     \
	)

#endif // !HBRS_MPL_FN_SELECT_FWD_MATLAB_HPP
