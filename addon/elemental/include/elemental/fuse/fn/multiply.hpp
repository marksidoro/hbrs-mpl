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
#include <elemental/detail/Ring.hpp>
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

template <typename A, typename B, typename C>
auto
multiply_impl(A const& a, B const& b, C c) {
	typedef Ring_t<std::decay_t<A>> Ring;
	typedef std::decay_t<Ring> _Ring_;
	using namespace hbrs::mpl;
	
	if (a.Width() != b.Height()) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< elemental::errinfo_matrix_sizes{{ {a.Height(), a.Width()}, {b.Height(), b.Width()} }}
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

struct multiply_impl_matrix_matrix {
	template <
		typename RingL,
		typename RingR,
		typename std::enable_if_t<
			!std::is_same<RingL, RingR>::value &&
			boost::mpl::is_not_void_<std::common_type_t<RingL, RingR>>::value
		>* = nullptr
	>
	auto
	operator()(matrix<RingL> const& a, matrix<RingR> const& b) const {
		using namespace hbrs::mpl;
		typedef std::common_type_t<RingL, RingR> Ring;
		
		//Replace with faster algorithm (e.g. Strassen algorithm) and use parallelization
		
		matrix<Ring> a_ct {a.m(), a.n()};
		for(El::Int j = 0; j < a.n(); ++j) {
			for(El::Int i = 0; i < a.m(); ++i) {
				a_ct.at({i,j}) = a.at({i,j});
			}
		}
		
		matrix<Ring> b_ct {b.m(), b.n()};
		for(El::Int j = 0; j < b.n(); ++j) {
			for(El::Int i = 0; i < b.m(); ++i) {
				b_ct.at({i,j}) = b.at({i,j});
			}
		}
		
		return make_matrix( multiply_impl(a_ct.data(), b_ct.data(), El::Matrix<Ring>{}) );
	}
	
	template <typename Ring>
	auto
	operator()(matrix<Ring> const& a, matrix<Ring> const& b) const {
		return make_matrix( multiply_impl(a.data(), b.data(), El::Matrix<Ring>{}) );
	}
};

struct multiply_impl_AbstractDistMatrix_AbstractDistMatrix {
	template <typename Ring>
	auto
	operator()(El::AbstractDistMatrix<Ring> const& a, El::AbstractDistMatrix<Ring> const& b) const {
		using namespace hbrs::mpl;
		
		BOOST_ASSERT(a.Grid() == b.Grid());
		// "El::MC, El::MR" as used by proxy in Elemental/src/blas_like/level3/Gemm/NN.hpp
		return multiply_impl(a, b, El::DistMatrix<Ring, El::MC, El::MR>{a.Grid()});
	}
};

struct multiply_impl_matrix_scv_vector {
	template <
		typename MatrixRing,
		typename Sequence,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Sequence>, hana::ext::std::vector_tag >::value
			//TODO: Check that MatrixRing and VectorRing (see below) are compatible!
		>* = nullptr
	>
	auto
	operator()(matrix<MatrixRing> const& a, mpl::scv<Sequence> const& b) const {
		using namespace hbrs::mpl;
		
		typedef typename std::remove_reference_t<Sequence>::value_type VectorRing;
		typedef std::decay_t<decltype( std::declval<MatrixRing>() * std::declval<VectorRing>() )> Ring;
		
		if ((*not_equal)(a.n(), b.size())) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrix_vector_exception{}
				<< elemental::errinfo_matrix_size{(*size)(a)}
				<< elemental::errinfo_vector_size{(El::Int)b.size()}
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
};

struct multiply_impl_matrix_scalar {
	template <
		typename Ring,
		typename std::enable_if_t< 
			!std::is_const< Ring >::value
		>* = nullptr
	>
	auto
	operator()(matrix<Ring> a, Ring const& b) const {
		El::Scale(b, a.data());
		return a;
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_MULTIPLY_IMPLS boost::hana::make_tuple(                                                      \
		elemental::detail::multiply_impl_matrix_matrix{},                                                              \
		elemental::detail::multiply_impl_matrix_scv_vector{},                                                          \
		elemental::detail::multiply_impl_matrix_scalar{},                                                              \
		elemental::detail::multiply_impl_AbstractDistMatrix_AbstractDistMatrix{}                                       \
	)

#endif // !ELEMENTAL_FUSE_FN_MULTIPLY_HPP
