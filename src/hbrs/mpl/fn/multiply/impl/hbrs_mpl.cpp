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

#include "hbrs_mpl.hpp"

#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template
auto
multiply_impl_rtsacv_scalar::operator()(rtsacv<double>, double const&) const;

template
auto
multiply_impl_rtsacv_scalar::operator()(rtsacv<float>, float const&) const;

template
auto
multiply_impl_scalar_rtsacv::operator()(double const&, rtsacv<double>) const;

template
auto
multiply_impl_scalar_rtsacv::operator()(float const&, rtsacv<float>) const;

template
decltype(auto)
multiply_impl_rtsarv_rtsacv::operator()(rtsarv<double> const&, rtsacv<double> const&) const;

template
decltype(auto)
multiply_impl_rtsarv_rtsacv::operator()(rtsarv<float> const&, rtsacv<float> const&) const;

template
decltype(auto)
multiply_impl_rtsacv_rtsarv::operator()(rtsacv<double> const&, rtsarv<double> const&) const;

template
decltype(auto)
multiply_impl_rtsacv_rtsarv::operator()(rtsacv<float> const&, rtsarv<float> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(rtsam<double,storage_order::row_major> const&, rtsam<double,storage_order::row_major> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(rtsam<float,storage_order::column_major> const&, rtsam<float,storage_order::column_major> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&, submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&, submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(rtsam<double,storage_order::row_major> const&, submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(rtsam<float,storage_order::column_major> const&, submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&, rtsam<double,storage_order::row_major> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&, rtsam<float,storage_order::column_major> const&) const;

template
decltype(auto)
multiply_impl_rtsarv_rtsam::operator()(rtsarv<double> const&, rtsam<double,storage_order::row_major> const&) const;

template
decltype(auto)
multiply_impl_rtsarv_rtsam::operator()(rtsarv<float> const&, rtsam<float,storage_order::column_major> const&) const;

template
decltype(auto)
multiply_impl_rtsarv_rtsam::operator()(rtsarv<double> const&, submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&) const;

template
decltype(auto)
multiply_impl_rtsarv_rtsam::operator()(rtsarv<float> const&, submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsacv::operator()(rtsam<double,storage_order::row_major> const&, rtsacv<double> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsacv::operator()(rtsam<float,storage_order::column_major> const&, rtsacv<float> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsacv::operator()(submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&, rtsacv<double> const&) const;

template
decltype(auto)
multiply_impl_rtsam_rtsacv::operator()(submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&, rtsacv<float> const&) const;

template
auto
multiply_impl_rtsam_scalar::operator()(rtsam<double,storage_order::row_major>, double const&) const;

template
auto
multiply_impl_rtsam_scalar::operator()(rtsam<float,storage_order::column_major>, float const&) const;

template
auto
multiply_impl_scalar_rtsam::operator()(double const&, rtsam<double,storage_order::row_major>) const;

template
auto
multiply_impl_scalar_rtsam::operator()(float const&, rtsam<float,storage_order::column_major>) const;

template
auto
multiply_impl_givens_rotation_rtsam::operator()(givens_rotation<double> const&, rtsam<double,storage_order::row_major> const&) const;

template
auto
multiply_impl_givens_rotation_rtsam::operator()(givens_rotation<float> const&, rtsam<float,storage_order::column_major> const&) const;

template
auto
multiply_impl_givens_rotation_rtsam::operator()(givens_rotation<double> const&, submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>, matrix_size<std::size_t, std::size_t>> const&) const;

template
auto
multiply_impl_givens_rotation_rtsam::operator()(givens_rotation<float> const&, submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>, matrix_size<std::size_t, std::size_t>> const&) const;

template
auto
multiply_impl_rtsam_givens_rotation::operator()(rtsam<double,storage_order::row_major> const&, givens_rotation<double> const&) const;

template
auto
multiply_impl_rtsam_givens_rotation::operator()(rtsam<float,storage_order::column_major> const&, givens_rotation<float> const&) const;

template
auto
multiply_impl_rtsam_givens_rotation::operator()(submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>, matrix_size<std::size_t, std::size_t>> const&, givens_rotation<double> const&) const;

template
auto
multiply_impl_rtsam_givens_rotation::operator()(submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>, matrix_size<std::size_t, std::size_t>> const&, givens_rotation<float> const&) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

