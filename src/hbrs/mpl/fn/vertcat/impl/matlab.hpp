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

#ifndef MATLAB_FUSE_FN_VERTCAT_HPP
#define MATLAB_FUSE_FN_VERTCAT_HPP

#include <hbrs/mpl/core/preprocessor.hpp>
#include <matlab/dt/exception.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct vertcat_impl_smr_smr {
	template<
		typename MatrixA,
		typename MatrixB,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<MatrixA>, matlab::matrix_tag >::value && 
			std::is_same< hana::tag_of_t<MatrixB>, matlab::matrix_tag >::value && 
			std::is_same< std::decay_t<MatrixA>, std::decay_t<MatrixB> >::value
		>* = nullptr
	>
	auto
	operator()(
		mpl::smr<MatrixA, int> const& a,
		mpl::smr<MatrixB, int> const& b
	) const {
		using namespace hbrs::mpl;
		
		auto a_sz = (*size)(a);
		auto b_sz = (*size)(b);
		
		if (a_sz != b_sz) {
			BOOST_THROW_EXCEPTION((
				incompatible_vectors_exception{} 
				<< matlab::errinfo_vector_sizes{ {a_sz, b_sz} }
			));
		}
		
		std::decay_t<MatrixA> c{2, a_sz};
		
		for(int i = 0; i < a_sz; ++i) {
			c[0][i] = a[i];
		}
		
		for(int i = 0; i < a_sz; ++i) {
			c[1][i] = b[i];
		}
		
		return c;
	}
};

struct vertcat_impl_matrix_smr {
	template<
		typename MatrixA,
		typename MatrixB,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<MatrixA>, matlab::matrix_tag >::value && 
			std::is_same< hana::tag_of_t<MatrixB>, matlab::matrix_tag >::value && 
			std::is_same< std::decay_t<MatrixA>, std::decay_t<MatrixB> >::value
		>* = nullptr
	>
	auto
	operator()(
		MatrixA const& a,
		mpl::smr<MatrixB, int> const& b
	) const {
		using namespace hbrs::mpl;
		
		auto a_sz = (*size)(a);
		auto a_m = (*m)(a_sz);
		auto a_n = (*n)(a_sz);
		auto b_sz = (*size)(b);
		
		if (a_n != b_sz) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrix_vector_exception{} 
				<< matlab::errinfo_matrix_size{a_sz}
				<< matlab::errinfo_vector_size{b_sz}
			));
		}
		
		std::decay_t<MatrixA> c{a_m+1, a_n};
		
		for(int i = 0; i < a_m; ++i) {
			for(int j = 0; j < a_n; ++j) {
				c[i][j] = (*at)(a, make_matrix_index(i, j));
			}
		}
		
		for(int i = 0; i < a_n; ++i) {
			c[a_m][i] = (*at)(b, i);
		}
		
		return c;
	}
};

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_VERTCAT_IMPLS boost::hana::make_tuple(                                                          \
		matlab::detail::vertcat_impl_smr_smr{},                                                                        \
		matlab::detail::vertcat_impl_matrix_smr{}                                                                      \
	)

#endif // !MATLAB_FUSE_FN_VERTCAT_HPP
