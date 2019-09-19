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

#ifndef HBRS_MPL_FN_SELECT_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_SELECT_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/submatrix.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/rtsacv.hpp>
#include <hbrs/mpl/dt/rtsarv.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename Matrix,
	typename Pair,
	typename Ring
>
static auto
select_rtsam_range_index_impl(
	Matrix const& a,
	Pair const& p,
	hana::basic_type<Ring>
) {
	decltype(auto) rows = p.first;
	decltype(auto) column = p.second;

	std::size_t length = rows.last() - rows.first() + 1;
	rtsacv<Ring> v{length};
	for (std::size_t i = 0; i < length; ++i) {
		(*at)(v, i) = (*at)(a, make_matrix_index(i + rows.first(), column));
	}
	return v;
}

template<
	typename Matrix,
	typename Pair,
	typename Ring
>
static auto
select_rtsam_index_range_impl(
	Matrix const& a,
	Pair const& p,
	hana::basic_type<Ring>
) {
	decltype(auto) row = p.first;
	decltype(auto) columns = p.second;

	std::size_t length = columns.last() - columns.first() + 1;
	rtsarv<Ring> v{length};
	for (std::size_t i = 0; i < length; ++i) {
		(*at)(v, i) = (*at)(a, make_matrix_index(row, i + columns.first()));
	}
	return v;
}

template<
	typename Ring,
	storage_order Order
>
decltype(auto)
select_impl_rtsam_range_index::operator()(
	rtsam<Ring,Order> const& a,
	std::pair<
		range<std::size_t,std::size_t>,
		std::size_t
	> const& rng
) const {
	return select_rtsam_range_index_impl(a, rng, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
decltype(auto)
select_impl_rtsam_range_index::operator()(
	submatrix<rtsam<Ring,Order>&,Offset,Size> const& a,
	std::pair<
		range<std::size_t,std::size_t>,
		std::size_t
	> const& rng
) const {
	return select_rtsam_range_index_impl(a, rng, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order
>
decltype(auto)
select_impl_rtsam_index_range::operator()(
	rtsam<Ring,Order> const& a,
	std::pair<
		std::size_t,
		range<std::size_t,std::size_t>
	> const& rng
) const {
	return select_rtsam_index_range_impl(a, rng, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
decltype(auto)
select_impl_rtsam_index_range::operator()(
	submatrix<rtsam<Ring,Order>&, Offset,Size> const& a,
	std::pair<
		std::size_t,
		range<std::size_t,std::size_t>
	> const& rng
) const {
	return select_rtsam_index_range_impl(a, rng, hana::type_c<Ring>);
}

template<typename Ring, storage_order Order>
auto
select_impl_rtsam_range_range::operator()(
	rtsam<Ring,Order>& a,
	std::pair<
		range<std::size_t, std::size_t>,
		range<std::size_t, std::size_t>
	> const& rngs
) const {
	auto rows    = rngs.first;
	auto columns = rngs.second;
	
	BOOST_ASSERT(   rows.last() < a.size().m());
	BOOST_ASSERT(columns.last() < a.size().n());
	return submatrix<
		rtsam<Ring,Order>&,
		matrix_index<std::size_t, std::size_t>,
		matrix_size<std::size_t, std::size_t>
	> {
		a,
		make_matrix_index(rows.first(), columns.first()),
		make_matrix_size(
			rows.last() - rows.first() + 1,
			columns.last() - columns.first() + 1
		)
	};
}

template<typename Ring>
auto
select_impl_rtsacv_range::operator() (rtsacv<Ring> const& v, range<std::size_t,std::size_t> const& rng) const {
	rtsacv<Ring> v2 {rng.last() - rng.first() + 1u };
	for (std::size_t i = 0; i < (*size)(v2); ++i) {
		(*at)(v2, i) = (*at)(v, i + rng.first());
	}
	return v;
}

template<typename Ring>
auto
select_impl_rtsarv_range::operator() (rtsarv<Ring> const& v, range<std::size_t,std::size_t> const& rng) const {
	rtsarv<Ring> v2 {rng.last() - rng.first() + 1u };
	for (std::size_t i = 0; i < (*size)(v2); ++i) {
		(*at)(v2, i) = (*at)(v, i + rng.first());
	}
	return v;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END


#endif // !HBRS_MPL_FN_SELECT_IMPL_HBRS_MPL_HPP
