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

#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>

MATLAB_NAMESPACE_BEGIN
namespace detail {

int const&
n_impl_emxArray::operator()(emxArray_real_T const& a) const { 
	return a.size[1];
}

int const&
n_impl_emxArray::operator()(emxArray_boolean_T const& a) const { 
	return a.size[1];
}

int const&
n_impl_matrix::operator()(matrix<real_T> const& a) const { 
	return a.n();
}

int const&
n_impl_matrix::operator()(matrix<boolean_T> const& a) const { 
	return a.n();
}

/* namespace detail */ }
MATLAB_NAMESPACE_END