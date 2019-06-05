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

#pragma once

#ifndef ELEMENTAL_DT_DIST_MATRIX_HPP
#define ELEMENTAL_DT_DIST_MATRIX_HPP

#include <elemental/config.hpp>
#include <elemental/fwd/dt/dist_matrix.hpp>
#include <elemental/dt/matrix.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_distribution.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/make.hpp>
#include <El.hpp>
#include <type_traits>
#include <initializer_list>

BOOST_HANA_NAMESPACE_BEGIN

template <typename Ring>
struct tag_of<El::AbstractDistMatrix<Ring>> {
	using type = ext::El::AbstractDistMatrix_tag;
};

// See Elemental/include/El/core.hpp
template<typename T, El::Dist U, El::Dist V, El::DistWrap W>
struct tag_of<El::DistMatrix<T, U, V, W>> {
	using type = ext::El::DistMatrix_tag;
};

template <>
struct make_impl<ext::El::DistMatrix_tag> {
	template <typename Ring, El::Dist U, El::Dist V, El::DistWrap W>
	static auto
	apply(
		El::Grid const& grid,
		basic_type<Ring>,
		hbrs::mpl::matrix_distribution<
			integral_constant<El::Dist, U>, integral_constant<El::Dist, V>, integral_constant<El::DistWrap, W>
		>,
		hbrs::mpl::matrix_size<El::Int, El::Int> sz
	) {
		El::DistMatrix<Ring, U, V, W> dmat{grid};
		dmat.Resize(sz.m(), sz.n());
		return dmat;
	}
	
	template <typename Ring>
	static auto
	apply(
		El::Grid const& grid,
		El::Matrix<Ring> local
	) {
		El::DistMatrix<std::remove_const_t<Ring>, El::STAR, El::STAR, El::ELEMENT> dmat{grid};
		dmat.Resize(local.Height(), local.Width());
		dmat.Matrix() = local;
		return dmat;
	}
	
	template <typename Ring>
	static decltype(auto)
	apply(
		El::Grid const& grid,
		elemental::matrix<Ring> local
	) {
		return apply(grid, std::move(local).data());
	}
};

BOOST_HANA_NAMESPACE_END

#endif // !ELEMENTAL_DT_DIST_MATRIX_HPP
