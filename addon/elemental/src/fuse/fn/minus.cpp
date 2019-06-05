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

#include <hbrs/mpl/fn/minus.hpp>

#include <elemental/dt/dist_matrix.hpp>
#include <elemental/dt/dist_vector.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace detail {

template auto minus_impl_matrix_matrix::operator()(matrix<float>,               matrix<float>               const&) const;
template auto minus_impl_matrix_matrix::operator()(matrix<El::Complex<float>>,  matrix<El::Complex<float>>  const&) const;
template auto minus_impl_matrix_matrix::operator()(matrix<double>,              matrix<double>              const&) const;
template auto minus_impl_matrix_matrix::operator()(matrix<El::Complex<double>>, matrix<El::Complex<double>> const&) const;

template auto minus_impl_DistMatrix_expand_expr_DistMatrix::operator()(
	El::DistMatrix<float>,
	mpl::expression<
		mpl::expand_t,
		std::tuple<
			dist_row_vector<El::DistMatrix<float>> const&,
			mpl::matrix_size<El::Int, El::Int> const&
		>
	>) const;
template auto minus_impl_DistMatrix_expand_expr_DistMatrix::operator()(
	El::DistMatrix<El::Complex<float>>,
	mpl::expression<
		mpl::expand_t,
		std::tuple<
			dist_row_vector<El::DistMatrix<El::Complex<float>>> const&,
			mpl::matrix_size<El::Int, El::Int> const&
		>
	>) const;
template auto minus_impl_DistMatrix_expand_expr_DistMatrix::operator()(
	El::DistMatrix<double>,
	mpl::expression<
		mpl::expand_t,
		std::tuple<
			dist_row_vector<El::DistMatrix<double>> const&,
			mpl::matrix_size<El::Int, El::Int> const&
		>
	>) const;
template auto minus_impl_DistMatrix_expand_expr_DistMatrix::operator()(
	El::DistMatrix<El::Complex<double>>,
	mpl::expression<
		mpl::expand_t,
		std::tuple<
			dist_row_vector<El::DistMatrix<El::Complex<double>>> const&,
			mpl::matrix_size<El::Int, El::Int> const&
		>
	>) const;

template auto minus_impl_matrix_scalar::operator()(matrix<float>,               float               const&) const;
template auto minus_impl_matrix_scalar::operator()(matrix<El::Complex<float>>,  El::Complex<float>  const&) const;
template auto minus_impl_matrix_scalar::operator()(matrix<double>,              double              const&) const;
template auto minus_impl_matrix_scalar::operator()(matrix<El::Complex<double>>, El::Complex<double> const&) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END