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

#include <hbrs/mpl/dt/matrix_index.hpp>

#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/multiply.hpp>

#include <boost/assert.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

/* NOTE: MATLAB is column-major. 
 * Ref.: https://www.mathworks.com/programs/products/files/matlab-coder-code-generation-quick-start-guide-r2015a.pdf
 */

real_T & 
at_impl_emxArray::operator()(emxArray_real_T & v, int d0) const {
	BOOST_ASSERT(d0 < v.allocatedSize);
	return v.data[d0];
}

boolean_T & 
at_impl_emxArray::operator()(emxArray_boolean_T & v, int d0) const {
	BOOST_ASSERT(d0 < v.allocatedSize);
	return v.data[d0];
}

real_T const& 
at_impl_emxArray::operator()(emxArray_real_T const& v, int d0) const {
	BOOST_ASSERT(d0 < v.allocatedSize);
	return v.data[d0];
}

boolean_T const& 
at_impl_emxArray::operator()(emxArray_boolean_T const& v, int d0) const {
	BOOST_ASSERT(d0 < v.allocatedSize);
	return v.data[d0];
}

real_T & 
at_impl_emxArray::operator()(emxArray_real_T & a, matrix_index<int,int> i) const {
	using namespace mpl;
	
	// changin' indices from row (C) to column (MATLAB) order!
	// row order would be:
	//  return a.data[m(i) * n(a) + n(i)];
	int i_ = (*plus)(multiply(n(i), m(a)), m(i));
	BOOST_ASSERT(i_ < a.allocatedSize);
	return a.data[i_];
}

boolean_T & 
at_impl_emxArray::operator()(emxArray_boolean_T & a, matrix_index<int,int> i) const {
	using namespace mpl;
	
	int i_ = (*plus)(multiply(n(i), m(a)), m(i));
	BOOST_ASSERT(i_ < a.allocatedSize);
	return a.data[i_];
}

real_T const& 
at_impl_emxArray::operator()(emxArray_real_T const& a, matrix_index<int,int> i) const {
	using namespace mpl;
	
	int i_ = (*plus)(multiply(n(i), m(a)), m(i));
	BOOST_ASSERT(i_ < a.allocatedSize);
	return a.data[i_];
}

boolean_T const& 
at_impl_emxArray::operator()(emxArray_boolean_T const& a, matrix_index<int,int> i) const {
	using namespace mpl;
	
	int i_ = (*plus)(multiply(n(i), m(a)), m(i));
	BOOST_ASSERT(i_ < a.allocatedSize);
	return a.data[i_];
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
