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

#ifndef HBRS_MPL_FN_TRANSPOSE_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_TRANSPOSE_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/detail/log.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename Matrix,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value 
	>*
>
auto
transpose_impl_el_matrix::operator()(Matrix && m) const {
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_matrix:begin";
	
	typedef decltype(m.at({0,0})) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	El::Matrix<_Ring_> b;
	El::Transpose(HBRS_MPL_FWD(m).data(), b);
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_matrix:end";
	return make_el_matrix(std::move(b));
}

template<
	typename Vector,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Vector>, el_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<Vector>, el_row_vector_tag >
	>*
>
auto
transpose_impl_el_vector::operator()(Vector && v) const {
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_vector:begin";
	typedef decltype(v.at(0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	El::Matrix<_Ring_> b;
	El::Transpose(HBRS_MPL_FWD(v).data(), b);
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_vector:end";
	return hana::make<
		std::conditional_t<
			std::is_same_v< hana::tag_of_t<Vector>, el_column_vector_tag >,
			el_row_vector_tag,
			el_column_vector_tag
		>
	>(std::move(b));
}

template<
	typename DistMatrix,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<DistMatrix>, el_dist_matrix_tag >::value 
	>*
>
auto
transpose_impl_el_dist_matrix::operator()(DistMatrix && m) const {
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_dist_matrix:begin";
	
	typedef decltype(m.data()) ElDistMatrix;
	typedef std::decay_t<ElDistMatrix> _ElDistMatrix_;
	typedef typename _ElDistMatrix_::transType Transposed;
	
	Transposed b{m.data().Grid()};
	El::Transpose(HBRS_MPL_FWD(m).data(), b);
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_dist_matrix:end";
	return make_el_dist_matrix(std::move(b));
}

template<
	typename DistVector,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<DistVector>, el_dist_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<DistVector>, el_dist_row_vector_tag >
	>*
>
auto
transpose_impl_el_dist_vector::operator()(DistVector && v) const {
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_dist_vector:begin";
	typedef decltype(v.data()) ElDistMatrix;
	typedef std::decay_t<ElDistMatrix> _ElDistMatrix_;
	typedef typename _ElDistMatrix_::transType Transposed;
	
	Transposed b{v.data().Grid()};
	El::Transpose(HBRS_MPL_FWD(v).data(), b);
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "transpose:el_dist_vector:end";
	return hana::make<
		std::conditional_t<
			std::is_same_v< hana::tag_of_t<DistVector>, el_dist_column_vector_tag >,
			el_dist_row_vector_tag,
			el_dist_column_vector_tag
		>
	>(std::move(b));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_TRANSPOSE_IMPL_ELEMENTAL_HPP
