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

#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

//TODO: Add more explicit template instantiations

template
decltype(auto)
select_impl_rtsam_range_index::operator()(
	rtsam<float,storage_order::column_major> const&,
	std::pair<
		range<std::size_t,std::size_t>,
		std::size_t
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_range_index::operator()(
	rtsam<double,storage_order::row_major> const&,
	std::pair<
		range<std::size_t,std::size_t>,
		std::size_t
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_range_index::operator()(
	submatrix<rtsam<double,storage_order::row_major>&,matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&,
	std::pair<
		range<std::size_t,std::size_t>,
		std::size_t
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_range_index::operator()(
	submatrix<rtsam<float,storage_order::column_major>&,matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&,
	std::pair<
		range<std::size_t,std::size_t>,
		std::size_t
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_index_range::operator()(
	rtsam<double,storage_order::row_major> const&,
	std::pair<
		std::size_t,
		range<std::size_t,std::size_t>
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_index_range::operator()(
	rtsam<float,storage_order::column_major> const&, 
	std::pair<
		std::size_t,
		range<std::size_t,std::size_t>
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_index_range::operator()(
	submatrix<rtsam<double,storage_order::row_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&,
	std::pair<
		std::size_t,
		range<std::size_t,std::size_t>
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_index_range::operator()(
	submatrix<rtsam<float,storage_order::column_major>&, matrix_index<std::size_t, std::size_t>,matrix_size<std::size_t, std::size_t>> const&,
	std::pair<
		std::size_t,
		range<std::size_t,std::size_t>
	> const&
) const;

template
auto
select_impl_rtsam_range_range::operator()(
	rtsam<double,storage_order::row_major>& a,
	std::pair<
		range<std::size_t, std::size_t>,
		range<std::size_t, std::size_t>
	> const&
) const;

template
auto
select_impl_rtsam_range_range::operator()(
	rtsam<float,storage_order::column_major>& a,
	std::pair<
		range<std::size_t, std::size_t>,
		range<std::size_t, std::size_t>
	> const&
) const;

template
decltype(auto)
select_impl_rtsam_index_offset::operator()(
	rtsam<double,storage_order::row_major> const& a,
	std::pair<
		matrix_index<std::size_t, std::size_t>,
		matrix_size<std::size_t, std::size_t>
	>  const& rng
) const;

template
decltype(auto)
select_impl_rtsam_index_offset::operator()(
	rtsam<float,storage_order::column_major> const& a,
	std::pair<
		matrix_index<std::size_t, std::size_t>,
		matrix_size<std::size_t, std::size_t>
	>  const& rng
) const;

template
auto
select_impl_rtsacv_range::operator() (
	rtsacv<float> const&,
	range<std::size_t,std::size_t> const&
) const;

template
auto
select_impl_rtsacv_range::operator() (
	rtsacv<double> const&,
	range<std::size_t,std::size_t> const&
) const;

template
auto
select_impl_rtsarv_range::operator() (
	rtsarv<float> const&,
	range<std::size_t,std::size_t> const&
) const;

template
auto
select_impl_rtsarv_range::operator() (
	rtsarv<double> const&,
	range<std::size_t,std::size_t> const&
) const;

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END
