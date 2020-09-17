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

#ifndef HBRS_MPL_FN_EQUAL_FWD_MATLAB_HPP
#define HBRS_MPL_FN_EQUAL_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/tuple.hpp>
#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/detail/matlab_cxn/fwd.hpp>
#endif // !HBRS_MPL_ENABLE_MATLAB
#include <complex>


HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

struct equal_impl_ml_creal_T {
	HBRS_MPL_API
	bool
	operator()(real_T const& lhs, creal_T const& rhs) const;
	
	HBRS_MPL_API
	bool
	operator()(creal_T const& lhs, real_T const& rhs) const;
	
	HBRS_MPL_API
	bool
	operator()(creal_T const& lhs, creal_T const& rhs) const;
	
	HBRS_MPL_API
	bool
	operator()(creal_T const& lhs, std::complex<real_T> const& rhs) const;
	
	HBRS_MPL_API
	bool
	operator()(std::complex<real_T> const& lhs, creal_T const& rhs) const;
};

#else // !HBRS_MPL_ENABLE_MATLAB
struct equal_impl_ml_creal_T{};
#endif // !HBRS_MPL_ENABLE_MATLAB

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_EQUAL_IMPLS_MATLAB boost::hana::make_tuple(                                                        \
		hbrs::mpl::detail::equal_impl_ml_creal_T{}                                                                     \
	)

#endif // !HBRS_MPL_FN_EQUAL_FWD_MATLAB_HPP
