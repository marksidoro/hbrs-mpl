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

#include <hbrs/mpl/fn/pca_filter.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <memory>
#include <vector>
#include <algorithm>
#include <boost/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>

extern "C" {
	#include <matlab/cxn/pca_filter_level0.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

MATLAB_NAMESPACE_BEGIN
namespace detail {

mpl::pca_filter_result<
	matlab::matrix<real_T> /* data */,
	matlab::column_vector<real_T> /* latent*/
>
pca_filter_impl::operator()(matlab::matrix<real_T> const& a, std::vector<bool> keep) const {
	using namespace hbrs::mpl;
	
	auto sz = (*size)(a);
	int m_ = (*m)(sz);
	int n_ = (*n)(sz);
	int filter_sz = boost::numeric_cast<int>((*size)(keep));
	
	BOOST_ASSERT(filter_sz == m_-1<n_ ? m_-1 : std::min(m_, n_));
	
	matlab::matrix<real_T> data;
	matlab::column_vector<real_T> latent;
	matlab::column_vector<boolean_T> filter{filter_sz}; /* row or column vector does not matter */
	
	for(int i = 0; i < filter_sz; ++i) {
		filter[i] = keep[i];
	}
	
	pca_filter_level0(
		&a.data(),
		&filter.data(),
		&data.data(),
		&latent.data()
	);
	
	return {data, latent};
}

/* namespace detail */ }
MATLAB_NAMESPACE_END