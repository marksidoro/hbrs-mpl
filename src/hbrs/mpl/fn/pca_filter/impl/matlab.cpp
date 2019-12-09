/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/pca_control.hpp>
#include <hbrs/mpl/dt/pca_filter_result.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/equal.hpp>

#include <hbrs/mpl/detail/log.hpp>

#include <boost/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <memory>
#include <vector>
#include <algorithm>

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/pca_filter_level0.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

pca_filter_result<
	ml_matrix<real_T> /* data */,
	ml_column_vector<real_T> /* latent*/
>
pca_filter_impl_ml_matrix::operator()(
	ml_matrix<real_T> const& a,
	std::vector<bool> const& keep,
	pca_control<bool,bool,bool> const& ctrl
) const {
	auto keep_sz = (*size)(keep);
	auto filter_sz = boost::numeric_cast<int>(keep_sz);
	
	ml_column_vector<boolean_T> filter(filter_sz); /* row or column vector does not matter */
	
	for(int i = 0; i < filter_sz; ++i) {
		filter[i] = keep[i];
	}
	
	return (*this)(a, filter, ctrl);
}

pca_filter_result<
	ml_matrix<real_T> /* data */,
	ml_column_vector<real_T> /* latent*/
>
pca_filter_impl_ml_matrix::operator()(
	ml_matrix<real_T> const& a,
	ml_column_vector<boolean_T> const& keep,
	pca_control<bool,bool,bool> const& ctrl
) const {
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca_filter:matlab:begin";
	HBRS_MPL_LOG_TRIVIAL(trace) << "A:" << loggable{a};
	HBRS_MPL_LOG_TRIVIAL(trace) << "keep:" << loggable{keep};
	
	auto sz = (*size)(a);
	int m_ = (*m)(sz);
	int n_ = (*n)(sz);
	
	auto keep_sz = (*size)(keep);
	
	auto DOF = m_ - (ctrl.center() ? 1 : 0);
	BOOST_ASSERT(keep_sz == ((DOF<n_ && ctrl.economy()) ? DOF : std::min(m_, n_)));
	
	ml_matrix<real_T> data;
	ml_column_vector<real_T> latent;
	
	pca_filter_level0(
		&a.data(),
		&keep.data(),
		ctrl.economy(),
		ctrl.center(),
		ctrl.normalize(),
		&data.data(),
		&latent.data()
	);
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "data:" << loggable{data};
	HBRS_MPL_LOG_TRIVIAL(trace) << "latent:" << loggable{latent};
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca_filter:matlab:end";
	return {data, latent};
}

//TODO: Uncomment and implement!
// pca_filter_result<
// 	matrix::Matrix<double> /* data */,
// 	vector::Vector<double> /* latent*/
// >
// pca_filter_impl_ml_matrix::operator()(
// 	matrix::Matrix<double> a,
// 	rtsav<bool> const& keep,
// 	pca_control<bool,bool> const& ctrl
// ) const {
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

pca_filter_result<
	ml_matrix<real_T> /* data */,
	ml_column_vector<real_T> /* latent*/
>
pca_filter_impl_ml_matrix::operator()(
	ml_matrix<real_T> const& a,
	std::function<bool(int)> const& keep,
	pca_control<bool,bool,bool> const& ctrl
) const {
	auto sz = (*size)(a);
	int m_ = (*m)(sz);
	int n_ = (*n)(sz);
	
	int filter_sz = m_-1<n_ ? m_-1 : std::min(m_, n_);
	ml_column_vector<boolean_T> filter(filter_sz); /* row or column vector does not matter */
	
	for(int i = 0; i < filter_sz; ++i) {
		filter[i] = keep(i);
	}
	
	return (*this)(a, filter, ctrl);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
