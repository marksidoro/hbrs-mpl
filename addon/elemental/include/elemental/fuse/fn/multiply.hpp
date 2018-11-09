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

#ifndef ELEMENTAL_FUSE_FN_MULTIPLY_HPP
#define ELEMENTAL_FUSE_FN_MULTIPLY_HPP

#include <elemental/config.hpp>
#include <El.hpp>
#include <boost/mpl/if.hpp>
#include <boost/hana/tuple.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/not_equal.hpp>
#include <hbrs/mpl/fwd/dt/scv.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <boost/mpl/void.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <elemental/dt/exception.hpp>
#include <type_traits>


ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;
namespace detail {

struct multiply_impl_Matrix_Matrix {
	template <
		typename RingL,
		typename RingR,
		typename std::enable_if_t<
			!std::is_same<RingL, RingR>::value &&
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<RingL> const& a, El::Matrix<RingR> const& b) const {
		using namespace hbrs::mpl;
		
		if (a.Width() != b.Height()) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrices_exception{}
				<< elemental::errinfo_matrix_sizes{{(*size)(a), (*size)(b)}}
			));
		}
		
		typedef std::common_type_t<RingL, RingR> Ring;
		
		El::Matrix<Ring> a_ct {a.Height(), a.Width()};
		El::Matrix<Ring> b_ct {b.Height(), b.Width()};
		El::Matrix<Ring> c{a.Height(), b.Width()};
		
		//Replace with faster algorithm (e.g. Strassen algorithm) and use parallelization
		
		for(El::Int j = 0; j < a.Width(); ++j) {
			for(El::Int i = 0; i < a.Height(); ++i) {
				*a_ct.Buffer(i,j) = *a.LockedBuffer(i,j);
			}
		}
		
		for(El::Int j = 0; j < b.Width(); ++j) {
			for(El::Int i = 0; i < b.Height(); ++i) {
				*b_ct.Buffer(i,j) = *b.LockedBuffer(i,j);
			}
		}
		
		El::Gemm(
			El::Orientation::NORMAL,
			El::Orientation::NORMAL,
			Ring(1),
			a_ct,
			b_ct,
			Ring(0), /* zero all entries in c because El::Matrix<> constructor does no zero initialisation */
			c
		);
		return c;
	}
	
	template <typename Ring>
	auto
	operator()(El::Matrix<Ring> const& a, El::Matrix<Ring> const& b) const {
		using namespace hbrs::mpl;
		
		if (a.Width() != b.Height()) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrices_exception{}
				<< elemental::errinfo_matrix_sizes{{(*size)(a), (*size)(b)}}
			));
		}
		
		El::Matrix<Ring> c{a.Height(), b.Width()};
		
		El::Gemm(
			El::Orientation::NORMAL,
			El::Orientation::NORMAL,
			Ring(1.0),
			a,
			b,
			Ring(0.0), /* zero all entries in c because El::Matrix<> constructor does no zero initialisation */
			c
		);
		return c;
	}
};

struct multiply_impl_Matrix_scv_vector {
	
	template <
		typename MatrixRing,
		typename Sequence,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Sequence>, hana::ext::std::vector_tag >::value
			//TODO: Check that MatrixRing and VectorRing (see below) are compatible!
		>* = nullptr
	>
	auto
	operator()(El::Matrix<MatrixRing> const& a, mpl::scv<Sequence> const& b) const {
		using namespace hbrs::mpl;
		
		typedef typename std::remove_reference_t<Sequence>::value_type VectorRing;
		typedef std::decay_t<decltype( std::declval<MatrixRing>() * std::declval<VectorRing>() )> Ring;
		
		if ((*not_equal)(a.Width(), b.size())) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrix_vector_exception{}
				<< elemental::errinfo_matrix_size{(*size)(a)}
				<< elemental::errinfo_vector_size{(El::Int)b.size()}
			));
		}
		
		std::vector<Ring> c(a.Height(), 0);
		
		for(El::Int j = 0; j < a.Width(); ++j ) {
			for(El::Int i = 0; i < a.Height(); ++i ) {
				(*at)(c,i) += (*at)(a, make_matrix_index(i,j)) * b.at(j);
			}
		}
		
		return make_scv(c);
	}
};

struct multiply_impl_Matrix_Scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(El::Matrix<Ring> a, Ring const& b) const {
		El::Scale(b, a);
		return a;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_MULTIPLY_IMPLS boost::hana::make_tuple(                                                      \
		elemental::detail::multiply_impl_Matrix_Matrix{},                                                              \
		elemental::detail::multiply_impl_Matrix_scv_vector{},                                                          \
		elemental::detail::multiply_impl_Matrix_Scalar{}                                                               \
	)

#endif // !ELEMENTAL_FUSE_FN_MULTIPLY_HPP
