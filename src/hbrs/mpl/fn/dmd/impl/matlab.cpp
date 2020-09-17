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

#include "matlab.hpp"
#ifdef HBRS_MPL_ENABLE_MATLAB

#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/ml_vector.hpp>
#include <hbrs/mpl/dt/dmd_control.hpp>
#include <hbrs/mpl/dt/dmd_result.hpp>
#include <hbrs/mpl/detail/log.hpp>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/dmd_level1.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

#define _DEF_DMD(lvl)                                                                                                  \
	HBRS_MPL_API                                                                                                       \
	dmd_result<                                                                                                        \
		ml_column_vector<creal_T> /* eigenvalues */,                                                                   \
		ml_matrix<creal_T>        /* modes */,                                                                         \
		ml_column_vector<creal_T> /* coefficients */                                                                   \
	>                                                                                                                  \
	dmd_impl_level ## lvl ## _ml_matrix::operator()(                                                                   \
		ml_matrix<real_T> const& x1,                                                                                   \
		ml_matrix<real_T> const& x2,                                                                                   \
		dmd_control<int> const& ctrl                                                                                   \
	) const {                                                                                                          \
		HBRS_MPL_LOG_TRIVIAL(debug) << "dmd:matlab:begin";                                                             \
		HBRS_MPL_LOG_TRIVIAL(trace) << "X1:" << loggable{x1};                                                          \
		HBRS_MPL_LOG_TRIVIAL(trace) << "X2:" << loggable{x2};                                                          \
                                                                                                                       \
		ml_column_vector<creal_T> eigenvalues;                                                                         \
		ml_matrix<creal_T> modes;                                                                                      \
		ml_column_vector<creal_T> coefficients;                                                                        \
		                                                                                                               \
		dmd_level ## lvl(                                                                                              \
			&x1.data(),                                                                                                \
			&x2.data(),                                                                                                \
			ctrl.target_rank(),                                                                                        \
			&eigenvalues.data(),                                                                                       \
			&modes.data(),                                                                                             \
			&coefficients.data()                                                                                       \
		);                                                                                                             \
		                                                                                                               \
		HBRS_MPL_LOG_TRIVIAL(trace) << "eigenvalues:" << loggable{eigenvalues};                                        \
		HBRS_MPL_LOG_TRIVIAL(trace) << "modes:" << loggable{modes};                                                    \
		HBRS_MPL_LOG_TRIVIAL(trace) << "coefficients:" << loggable{coefficients};                                      \
		HBRS_MPL_LOG_TRIVIAL(debug) << "dmd:matlab:end";                                                               \
		return { eigenvalues, modes, coefficients };                                                                   \
	}

_DEF_DMD(1)
#undef _DEF_DMD

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
