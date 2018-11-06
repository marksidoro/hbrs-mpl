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

#ifndef ELEMENTAL_FUSE_FN_MEAN_HPP
#define ELEMENTAL_FUSE_FN_MEAN_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>

#include <elemental/dt/matrix.hpp>
#include <elemental/dt/vector.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>

#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smrs.hpp>

#include <elemental/dt/exception.hpp>
#include <elemental/detail/Ring.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct mean_impl_smcs_Matrix {
	template <
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(mpl::smcs<Matrix> const& a) const {
		typedef Ring_t<std::decay_t<Matrix>> Ring;
		typedef std::decay_t<Ring> _Ring_;
		using namespace hbrs::mpl;
		auto a_sz = (*size)(a.data());
		auto a_m = (*m)(a_sz);
		auto a_n = (*n)(a_sz);
		
		if ((a_m == 0) || (a_n == 0)) {
			BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << elemental::errinfo_matrix_size{a_sz});
		}
		
		//TODO: optimize for column major vs row major storage order, possibly using BLAS/LAPACK functions
		
		// column mean
		El::Matrix<_Ring_> b;
		El::Zeros(b, 1, a_n);
		
		for(El::Int i = 0; i < a_m; ++i) {
			for(El::Int j = 0; j < a_n; ++j) {
				(*at)(b, make_matrix_index(0,j)) += (*at)(a.data(), make_matrix_index(i,j));
			}
		}
		
		for(El::Int j = 0; j < a_n; ++j) {
			(*at)(b, make_matrix_index(0,j)) /= a_m;
		}
		
		return make_row_vector(std::move(b));
	}
};

struct mean_impl_smrs_Matrix {
	template <
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, hana::ext::El::Matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(mpl::smrs<Matrix> const& a) const {
		typedef Ring_t<std::decay_t<Matrix>> Ring;
		typedef std::decay_t<Ring> _Ring_;
		using namespace hbrs::mpl;
		auto a_sz = (*size)(a.data());
		auto a_m = (*m)(a_sz);
		auto a_n = (*n)(a_sz);
		
		if ((a_m == 0) || (a_n == 0)) {
			BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << elemental::errinfo_matrix_size{a_sz});
		}
		
		//TODO: optimize for column major vs row major storage order, possibly using BLAS/LAPACK functions
		
		//row mean
		El::Matrix<_Ring_> b;
		El::Zeros(b, a_m, 1);
		
		for(El::Int i = 0; i < a_m; ++i) {
			for(El::Int j = 0; j < a_n; ++j) {
				(*at)(b, make_matrix_index(i,0)) += (*at)(a.data(), make_matrix_index(i,j));
			}
		}
		
		for(El::Int i = 0; i < a_m; ++i) {
			(*at)(b, make_matrix_index(i,0)) /= a_n;
		}
		
		return make_column_vector(std::move(b));
	}
};


/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_MEAN_IMPLS boost::hana::make_tuple(                                                          \
		elemental::detail::mean_impl_smcs_Matrix{},                                                                    \
		elemental::detail::mean_impl_smrs_Matrix{}                                                                     \
	)

#endif // !ELEMENTAL_FUSE_FN_MEAN_HPP
