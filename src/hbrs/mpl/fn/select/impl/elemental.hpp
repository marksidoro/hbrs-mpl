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

#ifndef HBRS_MPL_FN_SELECT_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_SELECT_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>

#include <boost/hana/first.hpp>
#include <boost/hana/second.hpp>
#include <boost/assert.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename Vector,
	typename std::enable_if_t<
		std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value
	>*
>
decltype(auto)
select_impl_el_column_vector::operator()(
	Vector && v,
	range<El::Int, El::Int>  const& rng
) const {
	return operator()(
		HBRS_MPL_FWD(v),
		El::IR{rng.first(), rng.last()+1}
	);
}

template<
	typename Vector,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value &&
		std::is_lvalue_reference<Vector>::value
	>*
>
auto
select_impl_el_column_vector::operator()(Vector && v, El::IR const& rng) const {
	return make_el_column_vector(v.data()(rng, El::ALL));
}

template<typename Ring>
auto
select_impl_el_column_vector::operator()(el_column_vector<Ring> && v, El::IR const& rng) const {
	El::Matrix<Ring> b;
	El::GetSubmatrix(std::move(v).data(), rng, El::ALL, b);
	
	return make_el_column_vector(b);
}

template<
	typename Matrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value ||
		std::is_same< hana::tag_of_t<Matrix>, el_dist_matrix_tag >::value
	>*
>
decltype(auto)
select_impl_el_matrix::operator()(
	Matrix && a,
	range<
		matrix_index<El::Int, El::Int>,
		matrix_index<El::Int, El::Int>
	>  const& rng
) const {
	return operator()(
		HBRS_MPL_FWD(a),
		std::make_pair(
			El::IR{rng.first().m(), rng.last().m()+1},
			El::IR{rng.first().n(), rng.last().n()+1}
		)
	);
}

template<
	typename Matrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value ||
		std::is_same< hana::tag_of_t<Matrix>, el_dist_matrix_tag >::value
	>*
>
decltype(auto)
select_impl_el_matrix::operator()(
	Matrix && a,
	std::pair<
		matrix_index<El::Int, El::Int>,
		matrix_size<El::Int, El::Int>
	>  const& rng
) const {
	return operator()(
		HBRS_MPL_FWD(a),
		std::make_pair(
			El::IR{
				hana::first(rng).m(), 
				hana::first(rng).m()+hana::second(rng).m()
			},
			El::IR{
				hana::first(rng).n(), 
				hana::first(rng).n()+hana::second(rng).n()
			}
		)
	);
}

template<typename Ring>
el_matrix<Ring>
select_impl_el_matrix::operator()(el_matrix<Ring> & a, std::pair<El::IR, El::IR> const& rng) const {
	return {El::View(a.data(), hana::first(rng), hana::second(rng))};
}

template<typename Ring>
el_matrix<Ring const>
select_impl_el_matrix::operator()(el_matrix<Ring> const& a, std::pair<El::IR, El::IR> const& rng) const {
	return {El::LockedView(a.data(), hana::first(rng), hana::second(rng))};
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
select_impl_el_matrix::operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> & a, std::pair<El::IR, El::IR> const& rng) const {
	return {El::View(a.data(), hana::first(rng), hana::second(rng))};
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
el_dist_matrix<Ring const, Columnwise, Rowwise, Wrapping>
select_impl_el_matrix::operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a, std::pair<El::IR, El::IR> const& rng) const {
	return {El::LockedView(a.data(), hana::first(rng), hana::second(rng))};
}

[[maybe_unused]] static bool
same_size_el(matrix_size<El::Int, El::Int> const& sz, std::pair<El::IR, El::IR> const& rng) {
	return hana::first(rng).beg == 0 && hana::first(rng).end == sz.m() &&
		hana::second(rng).beg == 0 && hana::second(rng).end == sz.n();
}

template<typename Ring>
el_matrix<Ring>
select_impl_el_matrix::operator()(el_matrix<Ring> && a, std::pair<El::IR, El::IR> const& rng) const {
	if (same_size_el((*size)(a), rng)) {
		return HBRS_MPL_FWD(a);
	} else {
		typedef std::decay_t<Ring> _Ring_;
		El::Matrix<_Ring_> b;
		El::GetSubmatrix(HBRS_MPL_FWD(a).data(), hana::first(rng), hana::second(rng), b);
		return {b};
	}
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
select_impl_el_matrix::operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> && a, std::pair<El::IR, El::IR> const& rng) const {
	if (same_size_el((*size)(a), rng)) {
		return HBRS_MPL_FWD(a);
	} else {
		El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> b{a.data().Grid()};
		El::GetSubmatrix(HBRS_MPL_FWD(a).data(), hana::first(rng), hana::second(rng), b);
		return {b};
	}
}

template<
	typename Vector,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Vector>, el_dist_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<Vector>, el_dist_row_vector_tag >
	>*
>
decltype(auto)
select_impl_el_dist_vector::operator()(
	Vector && v,
	range<El::Int, El::Int> const& rng
) const {
	return operator()(
		HBRS_MPL_FWD(v),
		El::IR{rng.first(), rng.last()+1}
	);
}

template<
	typename Vector,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Vector>, el_dist_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<Vector>, el_dist_row_vector_tag >
	>*
>
auto
select_impl_el_dist_vector::operator()(Vector && v, El::IR const& rng) const {
	return hana::make<hana::tag_of_t<Vector>>(
		(*select)(
			make_el_dist_matrix(v.data()),
			std::make_pair(rng, El::ALL)
		).data()
	);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_SELECT_IMPL_ELEMENTAL_HPP
