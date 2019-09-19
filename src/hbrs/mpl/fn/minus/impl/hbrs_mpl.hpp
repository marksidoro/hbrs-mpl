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

#ifndef HBRS_MPL_FN_MINUS_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_MINUS_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/submatrix.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/fn/minus.hpp>
#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename Ring,
	typename Matrix1,
	typename Matrix2
>
static auto
minus_rtsam_impl(Matrix1 const& m1, Matrix2 const& m2, hana::basic_type<Ring>) {
	auto m1_sz = (*size)(m1);
	auto m1_m = (*m)(m1_sz);
	auto m1_n = (*n)(m1_sz);
	auto m2_sz = (*size)(m2);
	
	BOOST_ASSERT((*equal)(m1_sz, m2_sz));

	typedef std::decay_t<Ring> _Ring_;
	//TODO: Choose best storage order
	rtsam<_Ring_, storage_order::row_major> result {m1_m, m1_n};
	//TODO: Optimize for loop to storage order of matrices
	for (std::size_t i = 0; i < m1_m; ++i) {
		for (std::size_t j = 0; j < m1_n; ++j) {
			(*at)(result, make_matrix_index(i, j)) = (*minus)(at(m1, make_matrix_index(i, j)), at(m2, make_matrix_index(i, j)));
		}
	}
	return result;
}

template<
	typename Ring,
	storage_order Order1,
	storage_order Order2
>
decltype(auto)
minus_impl_rtsam::operator()(rtsam<Ring,Order1> const& m1, rtsam<Ring,Order2> const& m2) const {
	return minus_rtsam_impl(m1,m2,hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order1,
	storage_order Order2,
	typename Offset,
	typename Size
>
decltype(auto)
minus_impl_rtsam::operator()(submatrix<rtsam<Ring,Order1>&, Offset, Size> const& m1, rtsam<Ring,Order2> const& m2) const {
	return minus_rtsam_impl(m1,m2,hana::type_c<Ring>);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_MINUS_IMPL_HBRS_MPL_HPP
