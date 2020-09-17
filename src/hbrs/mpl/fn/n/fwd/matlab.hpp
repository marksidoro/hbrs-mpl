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

#ifndef HBRS_MPL_FN_N_FWD_MATLAB_HPP
#define HBRS_MPL_FN_N_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

struct n_impl_emxArray {
	HBRS_MPL_API
	int const&
	operator()(emxArray_real_T const& a) const;

	HBRS_MPL_API
	int const&
	operator()(emxArray_creal_T const& a) const;

	HBRS_MPL_API
	int const&
	operator()(emxArray_boolean_T const& a) const;
};

struct n_impl_ml_matrix {
	HBRS_MPL_API
	int const&
	operator()(ml_matrix<real_T> const& a) const;

	HBRS_MPL_API
	int const&
	operator()(ml_matrix<creal_T> const& a) const;

	HBRS_MPL_API
	int const&
	operator()(ml_matrix<boolean_T> const& a) const;
};

#else
struct n_impl_emxArray {};
struct n_impl_ml_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_N_IMPLS_MATLAB boost::hana::make_tuple(                                                            \
		hbrs::mpl::detail::n_impl_emxArray{},                                                                          \
		hbrs::mpl::detail::n_impl_ml_matrix{}                                                                          \
	)

#endif // !HBRS_MPL_FN_N_FWD_MATLAB_HPP
