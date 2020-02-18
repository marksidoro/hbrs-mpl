/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_DMD_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_DMD_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/dmd_control.hpp>
#include <hbrs/mpl/dt/dmd_result.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>

#include <hbrs/mpl/detail/log.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/minimum.hpp>
#include <hbrs/mpl/fn/svd.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/inverse.hpp>
#include <hbrs/mpl/fn/eig.hpp>
#include <hbrs/mpl/fn/mldivide.hpp>
#include <hbrs/mpl/fn/complex.hpp>

#include <boost/assert.hpp>
#include <utility>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
namespace dmd_impl_el {

//TODO: Turn this code into a dedicated, generic dmd() implementation
/* C++ code is equivalent to MATLAB code in file src/hbrs/mpl/detail/matlab_cxn/impl/dmd_level1.m */
template <typename X1, typename X2, typename Control>
static auto
dmd(
	X1 const& x1,
	X2 const& x2,
	Control const& ctrl
) {
	HBRS_MPL_LOG_TRIVIAL(debug) << "dmd:elemental:begin";
	HBRS_MPL_LOG_TRIVIAL(trace) << "X1:" << loggable{x1};
	HBRS_MPL_LOG_TRIVIAL(trace) << "X2:" << loggable{x2};
	
	typedef decltype(x1.data().Get(0,0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	auto const x1_sz = (*size)(x1);
	auto const x1_m = (*m)(x1_sz);
	auto const x1_n = (*n)(x1_sz);
	
	auto const x2_sz = (*size)(x2);
	
	BOOST_ASSERT((*equal)(x1_sz,x2_sz));
	
	auto r = (*minimum)(ctrl.target_rank(), minimum(x1_m,x1_n));
	
	auto usv = (*svd)(x1, decompose_mode::economy);
	auto && U = (*at)(usv, svd_u{});
	auto && S = (*at)(usv, svd_s{});
	auto && V = (*at)(usv, svd_v{});
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "U:" << loggable{U};
	HBRS_MPL_LOG_TRIVIAL(trace) << "S:" << loggable{S};
	HBRS_MPL_LOG_TRIVIAL(trace) << "V:" << loggable{V};
	
	auto U_r = (*select)(
		std::move(U),
		std::make_pair(El::ALL, El::IR(0, r))
	);
	
	auto S_r = (*select)(
		std::move(S),
		std::make_pair(El::IR(0, r), El::IR(0, r))
	);
	
	auto V_r = (*select)(
		std::move(V),
		std::make_pair(El::ALL, El::IR(0, r))
	);
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "U_r:" << loggable{U_r};
	HBRS_MPL_LOG_TRIVIAL(trace) << "S_r:" << loggable{S_r};
	HBRS_MPL_LOG_TRIVIAL(trace) << "V_r:" << loggable{V_r};
	
	auto Atilde =
		(*multiply)(
			multiply(
				multiply(
					transpose(U_r),
					x2
				),
				V_r
			),
			inverse(diagonal_matrix_without_zeros_on_main_diagonal<decltype(S_r)>{S_r})
		);
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "A~:" << loggable{Atilde};
	
	auto VD = (*eig)(Atilde, eig_control<>{});
	auto && D   = (*at)(VD, eig_eigenvalues{});
	auto && W_r = (*at)(VD, eig_eigenvectors{});
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "D:" << loggable{D};
	HBRS_MPL_LOG_TRIVIAL(trace) << "W_r:" << loggable{W_r};
	
	auto modes =
		(*multiply)(
			multiply(
				multiply(x2, V_r),
				inverse(diagonal_matrix_without_zeros_on_main_diagonal<decltype(S_r)>{S_r})
			),
			W_r
		);
	
	auto eigenvalues = D;
	
	auto col1 = (*select)(std::move(x1), std::make_pair(El::ALL, 0));
    // TODO: Handle col1 having only zeros!
	auto coefficients = (*mldivide)(modes, complex(col1, _Ring_(0)));
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "col1:" << loggable{col1};
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "eigenvalues:" << loggable{eigenvalues};
	HBRS_MPL_LOG_TRIVIAL(trace) << "modes:" << loggable{modes};
	HBRS_MPL_LOG_TRIVIAL(trace) << "coefficients:" << loggable{coefficients};
	HBRS_MPL_LOG_TRIVIAL(debug) << "dmd:elemental:end";
	return make_dmd_result(eigenvalues, modes, coefficients);
}

/* namespace dmd_impl_el */ }

template <
	typename Ring,
	typename std::enable_if_t<
		!std::is_reference_v<Ring> &&
		!std::is_const_v<Ring> &&
		std::is_arithmetic_v<Ring>
	>*
>
decltype(auto)
dmd_impl_el_matrix::operator()(
	el_matrix<Ring> const& x1,
	el_matrix<Ring> const& x2,
	dmd_control<El::Int> const& ctrl
) const {
	return dmd_impl_el::dmd(x1, x2, ctrl);
}

template<
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename std::enable_if_t<
		!std::is_reference_v<Ring> &&
		!std::is_const_v<Ring> &&
		std::is_arithmetic_v<Ring>
	>*
>
decltype(auto)
dmd_impl_el_dist_matrix::operator()(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& x1,
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& x2,
	dmd_control<El::Int> const& ctrl
) const {
	return dmd_impl_el::dmd(x1, x2, ctrl);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_DMD_IMPL_ELEMENTAL_HPP
