/* Copyright (c) 2018 Jakob Meng, <jakobmeng@web.de>
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

#include <hbrs/mpl/fn/bidiag.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/bidiag_result.hpp>

extern "C" {
	#include <matlab/cxn/bidiag_level0.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

MATLAB_NAMESPACE_BEGIN
namespace detail {

#define _DEF_BIDIAG(lvl)                                                                                               \
	mpl::bidiag_result<                                                                                                \
		matlab::matrix<real_T> /* U */,                                                                                \
		matlab::matrix<real_T> /* B */,                                                                                \
		matlab::matrix<real_T> /* V */                                                                                 \
	>                                                                                                                  \
	bidiag_impl_level ## lvl::operator()(matlab::matrix<real_T> const& a, mpl::decompose_mode mode) const {            \
		using mpl::decompose_mode;                                                                                     \
		matlab::matrix<real_T> u, b, v;                                                                                \
		                                                                                                               \
		bidiag_level ## lvl(                                                                                           \
			&a.data(),                                                                                                 \
			mode == decompose_mode::complete                                                                           \
				? ::decompose_mode::complete                                                                           \
				: mode == decompose_mode::economy                                                                      \
					? ::decompose_mode::economy                                                                        \
					: ::decompose_mode::zero,                                                                          \
			&u.data(),                                                                                                 \
			&b.data(),                                                                                                 \
			&v.data()                                                                                                  \
		);                                                                                                             \
                                                                                                                       \
		return {u,b,v};                                                                                                \
	}

_DEF_BIDIAG(0)
#undef _DEF_BIDIAG

/* namespace detail */ }
MATLAB_NAMESPACE_END