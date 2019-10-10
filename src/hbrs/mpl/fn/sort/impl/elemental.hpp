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

#ifndef HBRS_MPL_FN_SORT_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_SORT_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>
#include <hbrs/mpl/dt/eig_result.hpp>

#include <hbrs/mpl/detail/sort_eig_result.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <
	typename Vector,
	typename Matrix,
	typename Predicate,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Vector>, el_column_vector_tag > &&
		std::is_same_v< hana::tag_of_t<Matrix>, el_matrix_tag >
	>*
>
auto
sort_impl_el_eig_result::operator()(eig_result<Vector,Matrix> const& r, Predicate && p) const {
	return sort_eig_result(r, HBRS_MPL_FWD(p));
}

template <
	typename Vector,
	typename Matrix,
	typename Predicate,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<Vector>, el_dist_column_vector_tag > &&
		std::is_same_v< hana::tag_of_t<Matrix>, el_dist_matrix_tag >
	>*
>
auto
sort_impl_el_eig_result::operator()(eig_result<Vector,Matrix> const& r, Predicate && p) const {
	typedef decltype(r.eigenvalues().data().Get(0,0)) RingV;
	typedef decltype(r.eigenvectors().data().Get(0,0)) RingM;
	typedef std::decay_t<RingV> _RingV_;
	typedef std::decay_t<RingM> _RingM_;
	
	//TODO: Implement distributed sort algorithm!
	
	//NOTE: A [STAR,STAR] distribution might be very memory-intensive!
	
	El::DistMatrix<_RingV_, El::STAR, El::STAR> eigenvalues = { r.eigenvalues().data() };
	El::DistMatrix<_RingM_, El::STAR, El::STAR> eigenvectors = { r.eigenvectors().data() };
	
	auto sorted = sort_eig_result(
		make_eig_result(
			make_el_column_vector(std::move(eigenvalues).Matrix()),
			make_el_matrix(std::move(eigenvectors).Matrix())
		),
		HBRS_MPL_FWD(p)
	);
	
	return make_eig_result(
		make_el_dist_column_vector(r.eigenvalues().data().Grid(), std::move(sorted.eigenvalues())),
		make_el_dist_matrix(r.eigenvectors().data().Grid(), std::move(sorted.eigenvectors()))
	);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_SORT_IMPL_ELEMENTAL_HPP
