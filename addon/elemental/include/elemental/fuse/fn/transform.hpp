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

#ifndef ELEMENTAL_FUSE_FN_TRANSFORM_HPP
#define ELEMENTAL_FUSE_FN_TRANSFORM_HPP

#include <elemental/config.hpp>
#include <elemental/dt/matrix.hpp>
#include <elemental/dt/vector.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smrs.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <type_traits>
#include <functional>
#include <vector>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct transform_impl_vector {
	template <
		typename Vector,
		typename F,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Vector>, column_vector_tag >::value ||
			std::is_same< hana::tag_of_t<Vector>, row_vector_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(Vector a, F && f) const {
		using namespace hbrs::mpl;
		
		for(El::Int i=0; i < (*size)(a); ++i) {
			a.at(i) = mpl::evaluate(f(a.at(i)));
		}
		
		return a;
	}
};

struct transform_impl_Matrix {
	template <
		typename Ring,
		typename F,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(El::Matrix<Ring> a, F && f) const {
		auto const ldim = a.LDim();
		for(El::Int j=0; j < a.Width(); ++j) {
			for(El::Int i=0; i < a.Height(); ++i) {
				decltype(auto) ref = a.Buffer()[i+j*ldim];
				ref = mpl::evaluate(f(ref));
			}
		}
		
		return a;
	}
};

struct transform_impl_smr_Matrix {
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(mpl::smr<Matrix, El::Int> const& a, F && f) const {
		typedef std::decay_t<decltype( mpl::evaluate( f(a.at(0)) ) )>  Result;
		std::vector<Result> b;
		b.reserve(a.length());
		
		for(El::Int i=0; i < a.length(); ++i) {
			b.push_back( mpl::evaluate(f(a.at(i))) );
		}
		
		return b;
	}
};

struct transform_impl_smc_Matrix {
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	auto
	operator()(mpl::smc<Matrix, El::Int> const& a, F && f) const {
		typedef std::decay_t<decltype( mpl::evaluate( f(a.at(0)) ) )>  Result;
		std::vector<Result> b;
		b.reserve(a.length());
		
		for(El::Int j=0; j < a.length(); ++j) {
			b.push_back( mpl::evaluate(f(a.at(j))) );
		}
		
		return b;
	}
};

struct transform_impl_smcs_smrs_Matrix {
	template <typename Sequence, typename F>
	static auto
	impl(Sequence const& s, F && f) {
		typedef std::decay_t<decltype( mpl::evaluate( f(s.at(0)) ) )>  Result;
		auto sz = (*mpl::size)(s);
		std::vector<Result> b;
		b.reserve(sz);
		
		for(El::Int i=0; i < sz; ++i) {
			b.push_back( mpl::evaluate(f(s.at(i))) );
		}
		
		return b;
	}
	
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	decltype(auto)
	operator()(mpl::smcs<Matrix> const& a, F && f) const {
		return impl(a, HBRS_MPL_FWD(f));
	}
	
	template <
		typename Matrix,
		typename F,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
			//TODO: Add invokable check for F?
		>* = nullptr
	>
	decltype(auto)
	operator()(mpl::smrs<Matrix> const& a, F && f) const {
		return impl(a, HBRS_MPL_FWD(f));
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_TRANSFORM_IMPLS boost::hana::make_tuple(                                                     \
		elemental::detail::transform_impl_vector{},                                                                    \
		elemental::detail::transform_impl_Matrix{},                                                                    \
		elemental::detail::transform_impl_smc_Matrix{},                                                                \
		elemental::detail::transform_impl_smcs_smrs_Matrix{},                                                          \
		elemental::detail::transform_impl_smr_Matrix{}                                                                 \
	)

#endif // !ELEMENTAL_FUSE_FN_TRANSFORM_HPP
