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

#ifndef ELEMENTAL_FUSE_FN_SELECT_HPP
#define ELEMENTAL_FUSE_FN_SELECT_HPP

#include <elemental/config.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <elemental/dt/matrix_range.hpp>
#include <elemental/dt/vector.hpp>
#include <type_traits>
#include <elemental/detail/Ring.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct select_impl_column_vector {
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Vector && v,
		mpl::range<El::Int, El::Int>  const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(v),
			El::IR{rng.first(), rng.last()+1}
		);
	}
	
	template<
		typename Vector,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value &&
			std::is_lvalue_reference<Vector>::value
		>* = nullptr
	>
	auto
	operator()(Vector && v, El::IR const& rng) const {
		return make_column_vector(v.data()(rng, El::ALL));
	}
	
	template<typename Ring>
	auto
	operator()(column_vector<Ring> && v, El::IR const& rng) const {
		using namespace hbrs::mpl;
		
		El::Matrix<Ring> b;
		El::GetSubmatrix(std::move(v).data(), rng, El::ALL, b);
		
		return make_column_vector(b);
	}
};

struct select_impl_Matrix {
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		mpl::range<
			mpl::matrix_index<El::Int, El::Int>,
			mpl::matrix_index<El::Int, El::Int>
		>  const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(a),
			std::make_pair(
				El::IR{rng.first().m(), rng.last().m()+1},
				El::IR{rng.first().n(), rng.last().n()+1}
			)
		);
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		std::pair<
			mpl::matrix_index<El::Int, El::Int>,
			mpl::matrix_size<El::Int, El::Int>
		>  const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(a),
			std::make_pair(
				El::IR{
					hana::first(rng).m(), 
					hana::first(rng).m()+hana::second(rng).m()
				},
				El::IR{
					hana::first(rng).n(), 
					hana::first(rng).n()+hana::second(rng).n()
				}
			)
		);
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	auto
	operator()(Matrix && a, std::pair<El::IR, El::IR> const& rng) const {
		using namespace hbrs::mpl;
		typedef Ring_t<std::decay_t<Matrix>> Ring;
		typedef std::decay_t<Ring> _Ring_;
		
		El::Matrix<_Ring_> b;
		
		if constexpr (std::is_const<std::remove_reference_t<Matrix>>::value) {
			El::LockedView(b, a, hana::first(rng), hana::second(rng));
		} else {
			El::View      (b, a, hana::first(rng), hana::second(rng));
		}
		
		return b;
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value &&
			!std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	auto
	operator()(Matrix && a, std::pair<El::IR, El::IR> const& rng) const {
		using namespace hbrs::mpl;
		typedef Ring_t<std::decay_t<Matrix>> Ring;
		typedef std::decay_t<Ring> _Ring_;
		
		El::Matrix<_Ring_> b;
		El::GetSubmatrix(a, hana::first(rng), hana::second(rng), b);
		return b;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_SELECT_IMPLS boost::hana::make_tuple(                                                        \
		elemental::detail::select_impl_column_vector{},                                                                \
		elemental::detail::select_impl_Matrix{}                                                                        \
	)

#endif // !ELEMENTAL_FUSE_FN_SELECT_HPP
