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

#ifndef HBRS_MPL_DETAIL_SORT_EIG_RESULT_IMPL_HPP
#define HBRS_MPL_DETAIL_SORT_EIG_RESULT_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/eig_result.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/assert.hpp>
#include <vector>
#include <algorithm>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename Vector,
	typename Matrix,
	typename Predicate
>
static auto
sort_eig_result(eig_result<Vector,Matrix> const& r, Predicate && p) {
	auto eigenvalues = r.eigenvalues();
	auto eigenvectors = r.eigenvectors();
	
	auto eigval_sz = (*size)(eigenvalues);
	auto eigvec_sz = (*size)(eigenvectors);
	auto eigvec_m = (*m)(eigvec_sz);
	auto eigvec_n = (*n)(eigvec_sz);
	
	BOOST_ASSERT((*equal)(eigval_sz, eigvec_n));
	BOOST_ASSERT(eigvec_m >= 0);
	
	//Ref.: https://stackoverflow.com/a/12399290/6490710
	std::vector<std::size_t> idx(eigval_sz);
    std::iota(idx.begin(), idx.end(), 0);
	std::sort(
		idx.begin(),
		idx.end(),
		[&](auto i1, auto i2) {
			return p(
				(*at)(eigenvalues, i1),
				(*at)(eigenvalues, i2)
			);
		}
	);
	
	//TODO: Implement faster algorithm with lower memory usage!
	for(std::size_t i = 0; i < idx.size(); ++i) {
		(*at)(eigenvalues, i) = (*at)(r.eigenvalues(), idx[i]);
	}
	
	for(std::size_t i = 0; i < idx.size(); ++i) {
		for(std::size_t j = 0; j < boost::numeric_cast<std::size_t>(eigvec_m); ++j) {
			(*at)(eigenvectors, make_matrix_index(j, i)) = (*at)(r.eigenvectors(), make_matrix_index(j,idx[i]));
		}
	}
	
	return make_eig_result(eigenvalues, eigenvectors);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_SORT_EIG_RESULT_IMPL_HPP
