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

#ifndef HBRS_MPL_FN_COMPLEX_FWD_MATLAB_HPP
#define HBRS_MPL_FN_COMPLEX_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_vector/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

struct complex_impl_ml_vector {
	HBRS_MPL_API
	ml_column_vector<creal_T>
	operator()(ml_column_vector<real_T> const& real, real_T const& imag) const;
	
	HBRS_MPL_API
	ml_column_vector<creal_T>
	operator()(ml_column_vector<real_T> const& real, ml_column_vector<real_T> const& imag) const;
};

#else
struct complex_impl_ml_vector {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_COMPLEX_IMPLS_MATLAB boost::hana::make_tuple(                                                      \
		hbrs::mpl::detail::complex_impl_ml_vector{}                                                                    \
	)

#endif // !HBRS_MPL_FN_COMPLEX_FWD_MATLAB_HPP
