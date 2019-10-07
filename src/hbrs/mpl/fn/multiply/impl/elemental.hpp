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

#ifndef HBRS_MPL_FN_MULTIPLY_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_MULTIPLY_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/scv.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/exception.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/not_equal.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template <typename A, typename B, typename C>
auto
multiply_el_matrix_el_matrix_impl(A const& a, B const& b, C c) {
	typedef decltype(c.Get(0,0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	if (a.Width() != b.Height()) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< errinfo_el_matrix_sizes{{ {a.Height(), a.Width()}, {b.Height(), b.Width()} }}
		));
	}
	
	c.Resize(a.Height(), b.Width());
	
	El::Gemm(
		El::Orientation::NORMAL,
		El::Orientation::NORMAL,
		_Ring_(1),
		a,
		b,
		_Ring_(0), /* zero all entries in c because El::Matrix<> constructor does no zero initialisation */
		c
	);
	
	return c;
}

template <typename A, typename B, typename C>
auto
multiply_el_matrix_el_vector_impl(A const& a, B const& b, C c) {
	typedef decltype(c.Get(0,0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	if (a.Width() != b.Height()) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< errinfo_el_matrix_sizes{{ {a.Height(), a.Width()}, {b.Height(), b.Width()} }}
		));
	}
	
	c.Resize(a.Height(), b.Width());
	
	El::Gemv(
		El::Orientation::NORMAL,
		_Ring_(1),
		a,
		b,
		_Ring_(0), /* zero all entries in c because El::Matrix<> constructor does no zero initialisation */
		c
	);
	
	return c;
}

template <
	typename RingL,
	typename RingR,
	typename std::enable_if_t<
		!std::is_same<RingL, RingR>::value &&
		boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
	>*
>
decltype(auto)
multiply_impl_el_matrix_el_matrix::operator()(el_matrix<RingL> const& a, el_matrix<RingR> const& b) const {
	typedef std::common_type_t<RingL, RingR> Ring;
	
	El::MatrixReadProxy<RingL, Ring> a_pxy = {a.data()};
	El::MatrixReadProxy<RingR, Ring> b_pxy = {b.data()};
	
	return make_el_matrix(
		multiply_el_matrix_el_matrix_impl(a_pxy.GetLocked(), b_pxy.GetLocked(), El::Matrix<Ring>{})
	);
}

template <typename Ring>
decltype(auto)
multiply_impl_el_matrix_el_matrix::operator()(el_matrix<Ring> const& a, el_matrix<Ring> const& b) const {
	return make_el_matrix(multiply_el_matrix_el_matrix_impl(a.data(), b.data(), El::Matrix<Ring>{}));
}

template <typename Ring>
auto
multiply_impl_el_matrix_el_vector::operator()(el_matrix<Ring> const& a, el_column_vector<Ring> const& b) const {
	return make_el_column_vector(multiply_el_matrix_el_vector_impl(a.data(), b.data(), El::Matrix<Ring>{}));
}

template <
	typename RingL, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap Wrapping,
	typename RingR, El::Dist ColumnwiseR, El::Dist RowwiseR,
	typename std::enable_if_t<
		std::is_same_v<RingL, RingR> ||
		boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
	>*
>
decltype(auto)
multiply_impl_el_dist_matrix_el_dist_matrix::operator()(
	el_dist_matrix<RingL, ColumnwiseL, RowwiseL, Wrapping> const& a,
	el_dist_matrix<RingR, ColumnwiseR, RowwiseR, Wrapping> const& b
) const {
	BOOST_ASSERT(a.data().Grid() == b.data().Grid());
	
	typedef std::common_type_t<RingL, RingR> Ring;
	
	El::DistMatrixReadProxy<RingL, Ring, ColumnwiseL, RowwiseL, Wrapping> a_pxy = {a.data()};
	El::DistMatrixReadProxy<RingR, Ring, ColumnwiseR, RowwiseR, Wrapping> b_pxy = {b.data()};
	
	return make_el_dist_matrix(
		multiply_el_matrix_el_matrix_impl(
			a_pxy.GetLocked(),
			b_pxy.GetLocked(),
			// "El::MC, El::MR" as used by proxy in Elemental/src/blas_like/level3/Gemm/NN.hpp
			El::DistMatrix<Ring, El::MC, El::MR, Wrapping>{a.data().Grid()}
		)
	);
}


template <
	typename Ring, El::Dist ColumnwiseL, El::Dist RowwiseL, El::DistWrap Wrapping,
	/*          */ El::Dist ColumnwiseR, El::Dist RowwiseR
>
auto
multiply_impl_el_dist_matrix_el_dist_vector::operator()(
	el_dist_matrix<Ring, ColumnwiseL, RowwiseL, Wrapping> const& a,
	el_dist_column_vector<Ring, ColumnwiseR, RowwiseR, Wrapping> const& b
) const {
	BOOST_ASSERT(a.data().Grid() == b.data().Grid());
	
	return make_el_dist_column_vector(
		multiply_el_matrix_el_vector_impl(
			a.data(),
			b.data(),
			El::DistMatrix<Ring, El::MC, El::MR, Wrapping>{a.data().Grid()}
		)
	);
}


template <
	typename MatrixRing,
	typename Sequence,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Sequence>, hana::ext::std::vector_tag >::value
		//TODO: Check that MatrixRing and VectorRing (see below) are compatible!
	>*
>
auto
multiply_impl_el_matrix_scv_vector::operator()(el_matrix<MatrixRing> const& a, scv<Sequence> const& b) const {
	typedef typename std::remove_reference_t<Sequence>::value_type VectorRing;
	typedef std::decay_t<decltype( std::declval<MatrixRing>() * std::declval<VectorRing>() )> Ring;
	
	if ((*not_equal)(a.n(), b.size())) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrix_vector_exception{}
			<< errinfo_el_matrix_size{(*size)(a)}
			<< errinfo_el_vector_size{(El::Int)b.size()}
		));
	}
	
	std::vector<Ring> c(a.m(), 0);
	
	for(El::Int j = 0; j < a.n(); ++j ) {
		for(El::Int i = 0; i < a.m(); ++i ) {
			(*at)(c,i) += (*at)(a, make_matrix_index(i,j)) * b.at(j);
		}
	}
	
	return make_scv(c);
}

template <
	typename Ring,
	typename std::enable_if_t< 
		!std::is_const< Ring >::value
	>*
>
auto
multiply_impl_el_matrix_scalar::operator()(el_matrix<Ring> a, Ring const& b) const {
	El::Scale(b, a.data());
	return a;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_MULTIPLY_IMPL_ELEMENTAL_HPP
