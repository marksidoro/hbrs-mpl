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

#ifndef HBRS_MPL_FN_SELECT_FWD_ELEMENTAL_HPP
#define HBRS_MPL_FN_SELECT_FWD_ELEMENTAL_HPP

#include <hbrs/mpl/config.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix/fwd.hpp>
	#include <hbrs/mpl/dt/el_vector/fwd.hpp>
	#include <hbrs/mpl/dt/el_dist_vector/fwd.hpp>
	#include <hbrs/mpl/dt/matrix_index/fwd.hpp>
	#include <hbrs/mpl/dt/matrix_size/fwd.hpp>
	#include <hbrs/mpl/dt/range/fwd.hpp>
#endif

#include <boost/hana/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

struct select_impl_el_column_vector {
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Vector && v,
		range<El::Int, El::Int>  const& rng
	) const;
	
	template<
		typename Vector,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Vector>, el_column_vector_tag >::value &&
			std::is_lvalue_reference<Vector>::value
		>* = nullptr
	>
	auto
	operator()(Vector && v, El::IR const& rng) const;
	
	template<typename Ring>
	auto
	operator()(el_column_vector<Ring> && v, El::IR const& rng) const;
};

struct select_impl_el_matrix {
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, el_dist_matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		range<
			matrix_index<El::Int, El::Int>,
			matrix_index<El::Int, El::Int>
		>  const& rng
	) const;
	
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, el_matrix_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, el_dist_matrix_tag >::value
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Matrix && a,
		std::pair<
			matrix_index<El::Int, El::Int>,
			matrix_size<El::Int, El::Int>
		>  const& rng
	) const;
	
	template<typename Ring>
	el_matrix<Ring>
	operator()(el_matrix<Ring> & a, std::pair<El::IR, El::IR> const& rng) const;
	
	template<typename Ring>
	el_matrix<Ring const>
	operator()(el_matrix<Ring> const& a, std::pair<El::IR, El::IR> const& rng) const;
	
	template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> & a, std::pair<El::IR, El::IR> const& rng) const;
	
	template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	el_dist_matrix<Ring const, Columnwise, Rowwise, Wrapping>
	operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a, std::pair<El::IR, El::IR> const& rng) const;
	
	template<typename Ring>
	el_matrix<Ring>
	operator()(el_matrix<Ring> && a, std::pair<El::IR, El::IR> const& rng) const;
	
	template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	operator()(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> && a, std::pair<El::IR, El::IR> const& rng) const;
};

struct select_impl_el_dist_vector {
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<Vector>, el_dist_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<Vector>, el_dist_row_vector_tag >
		>* = nullptr
	>
	decltype(auto)
	operator()(
		Vector && v,
		range<El::Int, El::Int> const& rng
	) const;
	
	template<
		typename Vector,
		typename std::enable_if_t<
			std::is_same_v< hana::tag_of_t<Vector>, el_dist_column_vector_tag > ||
			std::is_same_v< hana::tag_of_t<Vector>, el_dist_row_vector_tag >
		>* = nullptr
	>
	auto
	operator()(Vector && v, El::IR const& rng) const;
};

#else
struct select_impl_el_column_vector {};
struct select_impl_el_matrix {};
struct select_impl_el_dist_vector {};
#endif

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#define HBRS_MPL_FN_SELECT_IMPLS_ELEMENTAL boost::hana::make_tuple(                                                    \
		hbrs::mpl::detail::select_impl_el_column_vector{},                                                             \
		hbrs::mpl::detail::select_impl_el_dist_vector{},                                                               \
		hbrs::mpl::detail::select_impl_el_matrix{}                                                                     \
	)

#endif // !HBRS_MPL_FN_SELECT_FWD_ELEMENTAL_HPP
