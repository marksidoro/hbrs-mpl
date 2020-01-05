/* Copyright (c) 2016-2020 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_PCA_FILTER_FWD_MATLAB_HPP
#define HBRS_MPL_FN_PCA_FILTER_FWD_MATLAB_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/pca_control/fwd.hpp>
	#include <hbrs/mpl/dt/pca_filter_control/fwd.hpp>
	#include <hbrs/mpl/dt/pca_filter_result/fwd.hpp>
	#include <hbrs/mpl/dt/ml_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/ml_vector/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>
#include <vector>
#include <functional>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_MATLAB

struct pca_filter_impl_ml_matrix {
	pca_filter_result<
		ml_matrix<real_T> /* data */,
		ml_column_vector<real_T> /* latent*/
	>
	operator()(
		ml_matrix<real_T> const& a,
		std::vector<bool> const& keep,
		pca_filter_control<pca_control<bool,bool,bool>,bool> const& ctrl
	) const;
	
	pca_filter_result<
		ml_matrix<real_T> /* data */,
		ml_column_vector<real_T> /* latent*/
	>
	operator()(
		ml_matrix<real_T> const& a,
		ml_column_vector<boolean_T> const& keep,
		pca_filter_control<pca_control<bool,bool,bool>,bool> const& ctrl
	) const;
	
	pca_filter_result<
		ml_matrix<real_T> /* data */,
		ml_column_vector<real_T> /* latent*/
	>
	operator()(
		ml_matrix<real_T> const& a,
		ml_row_vector<boolean_T> const& keep,
		pca_filter_control<pca_control<bool,bool,bool>,bool> const& ctrl
	) const = delete /* TODO: implement... */;
	
	pca_filter_result<
		ml_matrix<real_T> /* data */,
		ml_column_vector<real_T> /* latent*/
	>
	operator()(
		ml_matrix<real_T> const& a,
		std::function<bool(int)> const& keep,
		pca_filter_control<pca_control<bool,bool,bool>,bool> const& ctrl
	) const;
};

#else
struct pca_filter_impl_ml_matrix {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_PCA_FILTER_IMPLS_MATLAB boost::hana::make_tuple(                                                   \
		hbrs::mpl::detail::pca_filter_impl_ml_matrix{}                                                                 \
	)

#endif // !HBRS_MPL_FN_PCA_FILTER_FWD_MATLAB_HPP
