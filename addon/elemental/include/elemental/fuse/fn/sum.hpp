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

#ifndef ELEMENTAL_FUSE_FN_SUM_HPP
#define ELEMENTAL_FUSE_FN_SUM_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>

#include <elemental/dt/matrix.hpp>
#include <elemental/dt/vector.hpp>
#include <elemental/dt/dist_matrix.hpp>
#include <elemental/dt/dist_vector.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smc.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/columns.hpp>

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

struct sum_impl_smcs_Matrix {
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
		
		// column sum
		El::Matrix<_Ring_> b;
		El::Zeros(b, 1, a_n);
		
		for(El::Int i = 0; i < a_m; ++i) {
			for(El::Int j = 0; j < a_n; ++j) {
				(*at)(b, make_matrix_index(0,j)) += (*at)(a.data(), make_matrix_index(i,j));
			}
		}
		
		return make_row_vector(std::move(b));
	}
};

struct sum_impl_smrs_Matrix {
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
		
		//row sum
		El::Matrix<_Ring_> b;
		El::Zeros(b, a_m, 1);
		
		for(El::Int i = 0; i < a_m; ++i) {
			for(El::Int j = 0; j < a_n; ++j) {
				(*at)(b, make_matrix_index(i,0)) += (*at)(a.data(), make_matrix_index(i,j));
			}
		}
		
		return make_column_vector(std::move(b));
	}
};

struct sum_impl_DistMatrix_columns {
	template <
		typename DistMatrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<DistMatrix>, hana::ext::El::DistMatrix_tag >::value ||
			std::is_same< hana::tag_of_t<DistMatrix>, hana::ext::El::AbstractDistMatrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(columns_expr<DistMatrix> const& expr) const {
		using namespace hbrs::mpl;
		
		typedef Ring_t<std::decay_t<DistMatrix>> Ring;
		typedef std::decay_t<Ring> _Ring_;
		
		auto const& in_dmat = expr.from;
		auto in_dmat_sz = (*size)(in_dmat);
		auto in_dmat_m = (*m)(in_dmat_sz);
		auto in_dmat_n = (*n)(in_dmat_sz);
		
		if ((in_dmat_m == 0) || (in_dmat_n == 0)) {
			BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << elemental::errinfo_matrix_size{in_dmat_sz});
		}
		
		//TODO: Add decision routine to choose between El::ELEMENT and El::BLOCK!
		El::DistMatrix<_Ring_> ones_dmat{in_dmat.Grid()};
		El::Ones(ones_dmat, in_dmat.Height(), 1);

		//TODO: Maybe "El::STAR, El::STAR" with "El::CIRC, El::CIRC"?
		El::DistMatrix<_Ring_, El::STAR, El::STAR, El::ELEMENT> sums_dmat{in_dmat.Grid()};
		sums_dmat.Resize(1, in_dmat.Width());
		
		El::Gemv(
			El::Orientation::TRANSPOSE,
			_Ring_(1),
			in_dmat,
			ones_dmat,
			_Ring_(0),
			sums_dmat
		);
		
		return dist_row_vector<std::decay_t<decltype(sums_dmat)>>{sums_dmat};
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_SUM_IMPLS boost::hana::make_tuple(                                                           \
		elemental::detail::sum_impl_smcs_Matrix{},                                                                     \
		elemental::detail::sum_impl_smrs_Matrix{},                                                                     \
		elemental::detail::sum_impl_DistMatrix_columns{}                                                               \
	)

#endif // !ELEMENTAL_FUSE_FN_SUM_HPP
