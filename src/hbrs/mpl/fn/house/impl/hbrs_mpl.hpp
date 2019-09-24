/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_HOUSE_IMPL_HBR_MPL_HPP
#define HBRS_MPL_FN_HOUSE_IMPL_HBR_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/rtsarv.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/house_result.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/power.hpp>
#include <type_traits>
#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

//TODO: Add unittest for house_impl_rtsacv
/*
 * Algorithm 5.1.1 (Householder Vector) on page 236
 * Given the real column vector x, this function computes the real
 * column vector v with v(0)=1 and the real number beta.
 */
template<typename Ring>
auto
house_impl_rtsacv::operator()(rtsacv<Ring> const& x) {
	typedef std::decay_t<Ring> _Ring_;
	
	/* x2m is a temporary which is written x(2:m) in the book and is equivalent to x(range(1,m-1)) in this code */
	auto x2m = (*select)(x, range<std::size_t,std::size_t>{1u, (*size)(x)-1u});
	auto sigma = (*multiply)(transpose(x2m), x2m);

	/* The vector ni is the vector x with the value 1 in its first row */
	rtsacv<_Ring_> ni = x;
	ni.at(0) = 1;
	
	_Ring_ beta = 0;

	if (sigma == 0 && x.at(0) >= 0) {
		beta = 0;
	} else if (sigma == 0 && x.at(0) < 0) {
		/* In the book beta is set to -2 but in the Errata it says that it should be +2 */
		beta = 2;
	} else {
		auto mi = std::sqrt(x.at(0) * x.at(0) + sigma);
		if (x.at(0) <= 0) {
			ni.at(0) = x.at(0) - mi;
		} else {
			ni.at(0) = -sigma / (x.at(0) + mi);
		}
		auto nisq = (*power)(ni.at(0), _Ring_(2)); // square of first element of ni
		beta = 2 * nisq / (sigma + nisq);
		ni = (*divide)(ni, ni.at(0));
	}
	
	return make_house_result(x, beta);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_HOUSE_IMPL_HBR_MPL_HPP
