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
decltype(auto)
multiply_impl_el(A const& a, B const& b, C c) {
	typedef decltype(a.data().Get(0,0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	if (a.n() != b.m()) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< errinfo_el_matrix_sizes{{ {a.m(), a.n()}, {b.m(), b.n()} }}
		));
	}
	
	c.Resize(a.m(), b.n());
	
	El::Gemm(
		El::Orientation::NORMAL,
		El::Orientation::NORMAL,
		_Ring_(1),
		a.data(),
		b.data(),
		_Ring_(0), /* zero all entries in c because El::Matrix<> constructor does no zero initialisation */
		c
	);
	
	return hana::make<hana::tag_of_t<A>>(c);
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
	
	//Replace with faster algorithm (e.g. Strassen algorithm) and use parallelization
	
	el_matrix<Ring> a_ct {a.m(), a.n()};
	for(El::Int j = 0; j < a.n(); ++j) {
		for(El::Int i = 0; i < a.m(); ++i) {
			a_ct.at({i,j}) = a.at({i,j});
		}
	}
	
	el_matrix<Ring> b_ct {b.m(), b.n()};
	for(El::Int j = 0; j < b.n(); ++j) {
		for(El::Int i = 0; i < b.m(); ++i) {
			b_ct.at({i,j}) = b.at({i,j});
		}
	}
	
	return multiply_impl_el(a_ct, b_ct, El::Matrix<Ring>{});
}

template <typename Ring>
decltype(auto)
multiply_impl_el_matrix_el_matrix::operator()(el_matrix<Ring> const& a, el_matrix<Ring> const& b) const {
	return multiply_impl_el(a, b, El::Matrix<Ring>{});
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
	// "El::MC, El::MR" as used by proxy in Elemental/src/blas_like/level3/Gemm/NN.hpp
	
	typedef std::common_type_t<RingL, RingR> Ring;
	return multiply_impl_el(
		a,
		b,
		El::DistMatrix<Ring, El::MC, El::MR, Wrapping>{a.data().Grid()}
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
