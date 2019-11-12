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

#ifndef HBRS_MPL_FN_ANY_OF_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_ANY_OF_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template<
	typename Ring,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_rtsam::operator()(
	rtsam<Ring,storage_order::column_major> const& a,
	UnaryPredicate && pred
) const {
	auto a_sz = (*size)(a);
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	for(std::size_t j = 0; j < a_n; ++j) {
		for(std::size_t i = 0; i < a_m; ++i) {
			if (evaluate(pred(at(a, make_matrix_index(i,j))))) {
				return true;
			}
		}
	}
	return false;
}

template<
	typename Ring,
	typename UnaryPredicate
	//TODO: Add invokable check for UnaryPredicate?
>
bool
any_of_impl_rtsam::operator()(
	rtsam<Ring,storage_order::row_major> const& a,
	UnaryPredicate && pred
) const {
	auto a_sz = (*size)(a);
	auto a_m = (*m)(a_sz);
	auto a_n = (*n)(a_sz);
	
	for(std::size_t i = 0; i < a_m; ++i) {
		for(std::size_t j = 0; j < a_n; ++j) {
			if (evaluate(pred(at(a, make_matrix_index(i,j))))) {
				return true;
			}
		}
	}
	return false;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_ANY_OF_IMPL_HBRS_MPL_HPP
