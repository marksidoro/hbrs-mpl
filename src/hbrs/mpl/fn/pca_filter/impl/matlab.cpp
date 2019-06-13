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
	#include <hbrs/mpl/detail/matlab_cxn/impl/pca_filter_level0.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

mpl::pca_filter_result<
	hbrs::mpl::ml_matrix<real_T> /* data */,
	hbrs::mpl::ml_column_vector<real_T> /* latent*/
>
pca_filter_impl::operator()(hbrs::mpl::ml_matrix<real_T> const& a, std::vector<bool> const& keep) const {
	using namespace hbrs::mpl;
	auto keep_sz = (*size)(keep);
	auto filter_sz = boost::numeric_cast<int>(keep_sz);
	
	hbrs::mpl::ml_column_vector<boolean_T> filter{filter_sz}; /* row or column vector does not matter */
	
	for(int i = 0; i < filter_sz; ++i) {
		filter[i] = keep[i];
	}
	
	return (*this)(a, filter);
}

mpl::pca_filter_result<
	hbrs::mpl::ml_matrix<real_T> /* data */,
	hbrs::mpl::ml_column_vector<real_T> /* latent*/
>
pca_filter_impl::operator()(hbrs::mpl::ml_matrix<real_T> const& a, hbrs::mpl::ml_column_vector<boolean_T> const& keep) const {
	using namespace hbrs::mpl;
	auto sz = (*size)(a);
	int m_ = (*m)(sz);
	int n_ = (*n)(sz);
	
	auto keep_sz = (*size)(keep);
	BOOST_ASSERT(keep_sz == m_-1<n_ ? m_-1 : std::min(m_, n_));
	
	hbrs::mpl::ml_matrix<real_T> data;
	hbrs::mpl::ml_column_vector<real_T> latent;
	
	pca_filter_level0(
		&a.data(),
		&keep.data(),
		&data.data(),
		&latent.data()
	);
	
	return {data, latent};
}

//TODO: Uncomment and implement!
// mpl::pca_filter_result<
// 	matrix::Matrix<double> /* data */,
// 	vector::Vector<double> /* latent*/
// >
// pca_filter_impl1::operator()(matrix::Matrix<double> a, mpl::rtsav<bool> const& keep) const {
// 	auto && dec = pca2::pca(a, true);
// 	auto && coeff = dec.coeff();
// 	auto && score = dec.score();
// 	auto && latent = dec.latent();
// 	auto && mu = dec.mean();
// 	
// 	auto && size = matrix::size_of(coeff);
// 	auto && m = std::get<0>(size);
// 	auto && n = std::get<1>(size);
// 	
// 	for(std::size_t i = 0; i < n; ++i) {
// 		if(!keep[i]) {
// 			for(std::size_t j = 0; j < m; ++j) {
// 				coeff[j][i] = 0;
// 			}
// 		}
// 	}
// 	
// 	typedef double Real;
// 	auto && reduced = matrix::plus<Real>(
// 		matrix::product<Real>(
// 			score, matrix::transpose<Real>(coeff)
// 		), 
// 		mu,
// 		matrix::VectorType::RowVector
// 	);
// 	
// 	return { reduced, latent };
// }

mpl::pca_filter_result<
	hbrs::mpl::ml_matrix<real_T> /* data */,
	hbrs::mpl::ml_column_vector<real_T> /* latent*/
>
pca_filter_impl::operator()(hbrs::mpl::ml_matrix<real_T> const& a, std::function<bool(int)> const& keep) const {
	using namespace hbrs::mpl;
	auto sz = (*size)(a);
	int m_ = (*m)(sz);
	int n_ = (*n)(sz);
	
	int filter_sz = m_-1<n_ ? m_-1 : std::min(m_, n_);
	hbrs::mpl::ml_column_vector<boolean_T> filter{filter_sz}; /* row or column vector does not matter */
	
	for(int i = 0; i < filter_sz; ++i) {
		filter[i] = keep(i);
	}
	
	return (*this)(a, filter);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END