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

#include "matlab.hpp"
#ifdef HBRS_MPL_ENABLE_MATLAB

#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/svd_result.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/svd_level0.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/svd_level1.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#define _DEF_SVD(lvl)                                                                                                  \
	svd_result<                                                                                                        \
		ml_matrix<real_T> /* U */,                                                                                     \
		ml_matrix<real_T> /* S */,                                                                                     \
		ml_matrix<real_T> /* V */                                                                                      \
	>                                                                                                                  \
	svd_impl_level ## lvl ## _ml_matrix::operator()(ml_matrix<real_T> const& a, decompose_mode mode) const {           \
		ml_matrix<real_T> u, s, v;                                                                                     \
		                                                                                                               \
		svd_level ## lvl(                                                                                              \
			&a.data(),                                                                                                 \
			mode == decompose_mode::complete                                                                           \
				? ::decompose_mode::complete                                                                           \
				: mode == decompose_mode::economy                                                                      \
					? ::decompose_mode::economy                                                                        \
					: ::decompose_mode::zero,                                                                          \
			&u.data(),                                                                                                 \
			&s.data(),                                                                                                 \
			&v.data()                                                                                                  \
		);                                                                                                             \
                                                                                                                       \
		return {u,s,v};                                                                                                \
	}

_DEF_SVD(0)
_DEF_SVD(1)
#undef _DEF_SVD

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
