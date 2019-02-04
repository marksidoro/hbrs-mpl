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
#include <elemental/fwd/dt/matrix.hpp>
#include <elemental/fwd/dt/dist_matrix.hpp>
#include <elemental/fwd/dt/vector.hpp>
#include <elemental/fwd/dt/dist_vector.hpp>
#include <elemental/dt/matrix_range.hpp>
#include <elemental/detail/Ring.hpp>

#include <hbrs/mpl/detail/add_const.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>

#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <boost/assert.hpp>

#include <El.hpp>
#include <type_traits>

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
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
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
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
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
			(
				std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value ||
				std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
			) &&
			std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	auto
	operator()(Matrix && a, std::pair<El::IR, El::IR> const& rng) const {
		static constexpr auto a_is_const = std::is_const<std::remove_reference_t<Matrix>>::value;
		BOOST_ASSERT(a.Locked() ? a_is_const : true);
		/* NOTE: Not necessarily true for e.g. El::(Dist)Matrix<double> once both are
		 *       properly wrapped in elemental::(dist_)matrix this should be targeted!
		 */
		
		if constexpr (a_is_const) {
			/* type cast to const required here because El::LockedView
			 * returns El::(Dist)Matrix<double> with .Locked() == false!
			 */
			return mpl::detail::add_const(
				El::LockedView(a, hana::first(rng), hana::second(rng))
			);
		} else {
			return
				El::View      (a, hana::first(rng), hana::second(rng));
		}
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	auto
	make_like(Matrix && a) const {
		typedef std::decay_t<Matrix> _Matrix_;
		return _Matrix_{};
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
		>* = nullptr
	>
	auto
	make_like(Matrix && a) const {
		typedef std::decay_t<Matrix> _Matrix_;
		return _Matrix_{a.Grid()};
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t<
			(
				std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value ||
				std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
			) &&
			!std::is_lvalue_reference<Matrix>::value
		>* = nullptr
	>
	auto
	operator()(Matrix && a, std::pair<El::IR, El::IR> const& rng) const {
		using namespace hbrs::mpl;
		typedef std::decay_t<Matrix> _Matrix_;
		typedef Ring_t<_Matrix_> Ring;
		static_assert(!std::is_reference<Ring>::value && !std::is_const<Ring>::value, "");
		
		auto const a_sz = (*size)(a);
		auto const a_m = (*m)(a_sz);
		auto const a_n = (*n)(a_sz);
		
		if (hana::first(rng).beg == 0 && hana::first(rng).end == a_m &&
			hana::second(rng).beg == 0 && hana::second(rng).end == a_n) {
			return HBRS_MPL_FWD(a);
		} else {
			auto b = make_like(a);
			El::GetSubmatrix(HBRS_MPL_FWD(a), hana::first(rng), hana::second(rng), b);
			return b;
		}
	}
};

struct select_impl_dist_column_vector {
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		dist_column_vector<Matrix> && v,
		mpl::range<El::Int, El::Int> const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(v),
			El::IR{rng.first(), rng.last()+1}
		);
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		dist_column_vector<Matrix> const& v,
		mpl::range<El::Int, El::Int> const& rng
	) const {
		return operator()(
			HBRS_MPL_FWD(v),
			El::IR{rng.first(), rng.last()+1}
		);
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(dist_column_vector<Matrix> && v, El::IR const& rng) const {
		auto m = select_impl_Matrix{}(v.data(), std::make_pair(rng, El::ALL));
		return dist_column_vector<decltype(m)>{m};
	}
	
	template<
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::DistMatrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(dist_column_vector<Matrix> const& v, El::IR const& rng) const {
		auto m = (*mpl::select)(v.data(), std::make_pair(rng, El::ALL));
		return dist_column_vector<decltype(m)>{m};
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_SELECT_IMPLS boost::hana::make_tuple(                                                        \
		elemental::detail::select_impl_column_vector{},                                                                \
		elemental::detail::select_impl_dist_column_vector{},                                                           \
		elemental::detail::select_impl_Matrix{}                                                                        \
	)

#endif // !ELEMENTAL_FUSE_FN_SELECT_HPP
