/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_SUM_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_SUM_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smc.hpp>
#include <hbrs/mpl/dt/exception.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/columns.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Matrix,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Matrix>, el_matrix_tag >
	>*
>
auto
sum_impl_smcs_el_matrix::operator()(smcs<Matrix> const& a) const {
	typedef decltype(a.data().at({0,0})) Ring;
	typedef std::decay_t<Ring> _Ring_;
	auto a_sz = (*size)(a.data());
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	if ((a_m == 0) || (a_n == 0)) {
		BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_el_matrix_size{a_sz});
	}
	
	//TODO: optimize for column major vs row major storage order, possibly using BLAS/LAPACK functions
	
	// column sum
	el_row_vector<_Ring_> b{a_n};
	
	for(El::Int i = 0; i < a_m; ++i) {
		for(El::Int j = 0; j < a_n; ++j) {
			(*at)(b,j) += (*at)(a.data(), make_matrix_index(i,j));
		}
	}
	
	return b;
}

template <
	typename Matrix,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value
	>*
>
auto
sum_impl_smrs_el_matrix::operator()(smrs<Matrix> const& a) const {
	typedef decltype(a.data().at({0,0})) Ring;
	typedef std::decay_t<Ring> _Ring_;
	auto a_sz = (*size)(a.data());
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	if ((a_m == 0) || (a_n == 0)) {
		BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_el_matrix_size{a_sz});
	}
	
	//TODO: optimize for column major vs row major storage order, possibly using BLAS/LAPACK functions
	
	//row sum
	el_column_vector<_Ring_> b{a_m};
	
	for(El::Int i = 0; i < a_m; ++i) {
		for(El::Int j = 0; j < a_n; ++j) {
			(*at)(b, i) += (*at)(a.data(), make_matrix_index(i,j));
		}
	}
	
	return b;
}

template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
static auto
sum_impl_el_dist_matrix_columns_impl(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& from) {
	typedef std::decay_t<Ring> _Ring_;
	
	auto const& in_dmat = from.data();
	auto in_dmat_sz = (*size)(from);
	auto in_dmat_m = (*m)(in_dmat_sz);
	auto in_dmat_n = (*n)(in_dmat_sz);
	
	if ((in_dmat_m == 0) || (in_dmat_n == 0)) {
		BOOST_THROW_EXCEPTION(incompatible_matrix_exception{} << errinfo_el_matrix_size{in_dmat_sz});
	}
	
	El::DistMatrix<_Ring_, Columnwise, Rowwise, Wrapping> ones_dmat{in_dmat.Grid()};
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
	
	return make_el_dist_row_vector(sums_dmat);
}

template <
	typename DistMatrix,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >
	>*
>
auto
sum_impl_el_dist_matrix_columns::operator()(
	expression<
		columns_t,
		hana::tuple<DistMatrix>
	> const& expr
) const {
	return sum_impl_el_dist_matrix_columns_impl(hana::at_c<0>(expr.operands()));
}

template <
	typename DistMatrix,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >
	>*
>
auto
sum_impl_el_dist_matrix_columns::operator()(
	expression<
		columns_t,
		hana::tuple<DistMatrix>
	> && expr
) const {
	return sum_impl_el_dist_matrix_columns_impl(hana::at_c<0>(HBRS_MPL_FWD(expr).operands()));
}


/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_SUM_IMPL_ELEMENTAL_HPP
