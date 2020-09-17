/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
 * Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
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

#include "hbrs_mpl.hpp"

#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/fn/almost_equal.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/not_equal.hpp>
#include <hbrs/mpl/dt/almost_equal_control.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

HBRS_MPL_API
bool
almost_equal_impl_rtsam::operator()(
	rtsam<double,storage_order::row_major> const& lhs,
	rtsam<double,storage_order::row_major> const& rhs,
	almost_equal_control<int,int> const& ctrl
) const {
	auto lhs_sz = (*size)(lhs);
	auto lhs_m = (*m)(lhs_sz);
	auto lhs_n = (*n)(lhs_sz);
	auto rhs_sz = (*size)(rhs);
	
	if ((*not_equal)(lhs_sz, rhs_sz)) {
		return false;
	}
	
	for (std::size_t i = 0; i < lhs_m; ++i) {
		for (std::size_t j = 0; j < lhs_n; ++j) {
			if (!(*almost_equal)(
				lhs.at(make_matrix_index(i, j)),
				rhs.at(make_matrix_index(i, j)),
				ctrl
			)) {
				return false;
			}
		}
	}
	return true;
}

HBRS_MPL_API
bool
almost_equal_impl_rtsam::operator()(
	rtsam<double,storage_order::column_major> const& lhs,
	rtsam<double,storage_order::column_major> const& rhs,
	almost_equal_control<int,int> const& ctrl
) const {
	auto lhs_sz = (*size)(lhs);
	auto lhs_m = (*m)(lhs_sz);
	auto lhs_n = (*n)(lhs_sz);
	auto rhs_sz = (*size)(rhs);
	
	if ((*not_equal)(lhs_sz, rhs_sz)) {
		return false;
	}
	
	for (std::size_t i = 0; i < lhs_n; ++i) {
		for (std::size_t j = 0; j < lhs_m; ++j) {
			if (!(*almost_equal)(
				lhs.at(make_matrix_index(i, j)),
				rhs.at(make_matrix_index(i, j)),
				ctrl
			)) {
				return false;
			}
		}
	}
	return true;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

