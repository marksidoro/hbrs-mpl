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

#ifndef HBRS_MPL_DT_EL_DIST_MATRIX_IMPL_HPP
#define HBRS_MPL_DT_EL_DIST_MATRIX_IMPL_HPP

#include "fwd.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_distribution.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/assert.hpp>
#include <El.hpp>
#include <type_traits>
#include <initializer_list>

#ifdef EL_HAVE_SCALAPACK
	#pragma message "CAUTION: Elemental has been compiled with ScaLAPACK support. USE AT YOUR OWN RISK!"
	/* NOTE:
	 * If one MPI process is not part of El::Grid, then ScaLAPACK throws an error in [1]:
	 *    BLACS ERROR 'Cannot define a BLACS system handle based on MPI_COMM_NULL'
	 *    from {-1,-1}, pnum=460, Contxt=-1, on line 18 of file 'scalapack-2.0.2/BLACS/SRC/sys2blacs_.c'.
	 *
	 * Backtrace:
	 * During grid initialization in Grid::SetUpGrid() [2], this code is executed if any process is part of the grid:
	 * 
	 *    if( InGrid() ) {
	 *      ...
	 *    } else {
	 *      ...
	 *      vcComm_     = mpi::COMM_NULL;
	 *      ...
	 *    }
	 *    #ifdef EL_HAVE_SCALAPACK
	 *       blacsVCHandle_ = blacs::Handle( vcComm_.comm );
	 * 
	 * blacs::Handle() then calls ScaLAPACK's Csys2blacs_handle() function with MPI_COMM_NULL,
	 * which then causes the above-mentioned BLACS ERROR.
	 *
	 * References:
	 * [1] scalapack/BLACS/SRC/sys2blacs_.c
	 * [2] Elemental/src/core/Grid.cpp
	 */
#endif //!EL_HAVE_SCALAPACK

HBRS_MPL_NAMESPACE_BEGIN

template<
	typename Ring = double,
	El::Dist Columnwise = El::MC,
	El::Dist Rowwise = El::MR,
	El::DistWrap Wrapping = El::ELEMENT
>
struct el_dist_matrix {
	template<typename Ring_, El::Dist Columnwise_, El::Dist Rowwise_, El::DistWrap Wrapping_>
	friend struct el_dist_matrix;
	
	template<
		typename Ring_ = Ring,
		El::Dist Columnwise_ = Columnwise,
		El::Dist Rowwise_ = Rowwise,
		El::DistWrap Wrapping_ = Wrapping,
		typename std::enable_if_t<
			std::is_same_v<std::remove_const_t<Ring>, Ring_>
		>* = nullptr
	>
	el_dist_matrix(El::DistMatrix<Ring_, Columnwise_, Rowwise_, Wrapping_> data) : data_{data} {
		BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	}
	
	el_dist_matrix(El::Grid const& grid, El::Int m, El::Int n) : data_{grid} {
		data_.Resize(m, n);
		El::Zero(data_);
	}
	
	template<
		typename Ring_ = Ring,
		El::Dist Columnwise_ = Columnwise,
		El::Dist Rowwise_ = Rowwise,
		El::DistWrap Wrapping_ = Wrapping,
		typename std::enable_if_t<
			std::is_same_v<std::remove_const_t<Ring>, Ring_>
		>* = nullptr
	>
	el_dist_matrix(el_dist_matrix<Ring_, Columnwise_, Rowwise_, Wrapping_> const& o) : data_{o.data_} {}
	
	el_dist_matrix(el_dist_matrix const&) = default;
	el_dist_matrix(el_dist_matrix &&) = default;
	
	el_dist_matrix&
	operator=(el_dist_matrix const&) = default;
	el_dist_matrix&
	operator=(el_dist_matrix &&) = default;
	
	auto
	m() const {
		return data_.Height();
	}
	
	auto
	n() const {
		return data_.Width();
	}

	matrix_size<El::Int, El::Int>
	size() const {
		return { m(), n() };
	}
	
	decltype(auto)
	data() & { return (data_); };

	decltype(auto)
	data() const& { return (data_); }; 

	decltype(auto)
	data() && { return HBRS_MPL_FWD(data_); }; 

private:
	El::DistMatrix<
		std::remove_const_t<Ring> /* elemental handles constness using a boolean El::Matrix<>.Locked() */,
		Columnwise, Rowwise, Wrapping
	> data_;
};

HBRS_MPL_NAMESPACE_END

BOOST_HANA_NAMESPACE_BEGIN

/* Ref.: Elemental/include/El/core.hpp */
template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
struct tag_of<hbrs::mpl::el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>> {
	using type = hbrs::mpl::el_dist_matrix_tag;
};

template <>
struct make_impl<hbrs::mpl::el_dist_matrix_tag> {
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	static hbrs::mpl::el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	apply(
		El::Grid const& grid,
		basic_type<Ring>,
		hbrs::mpl::matrix_distribution<
			integral_constant<El::Dist, Columnwise>,
			integral_constant<El::Dist, Rowwise>,
			integral_constant<El::DistWrap, Wrapping>
		>,
		hbrs::mpl::matrix_size<El::Int, El::Int> sz
	) {
		return { grid, sz.m(), sz.n() };
	}
	
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	static hbrs::mpl::el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping>
	apply(El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> dmat) {
		return { dmat };
	}
	
	template <typename Ring>
	static hbrs::mpl::el_dist_matrix<Ring, El::STAR, El::STAR, El::ELEMENT>
	apply(
		El::Grid const& grid,
		El::Matrix<Ring> local
	) {
		El::DistMatrix<Ring, El::STAR, El::STAR, El::ELEMENT> dmat{grid};
		dmat.Resize(local.Height(), local.Width());
		dmat.Matrix() = local;
		return { dmat };
	}
	
	template <typename Ring>
	static decltype(auto)
	apply(
		El::Grid const& grid,
		hbrs::mpl::el_matrix<Ring> local
	) {
		return apply(grid, std::move(local).data());
	}
	
	template <
		typename Ring,
		El::Dist FromColumnwise, El::Dist FromRowwise, El::DistWrap FromWrapping,
		El::Dist ToColumnwise, El::Dist ToRowwise, El::DistWrap ToWrapping
	>
	static hbrs::mpl::el_dist_matrix<Ring, ToColumnwise, ToRowwise, ToWrapping>
	apply(
		hbrs::mpl::el_dist_matrix<Ring, FromColumnwise, FromRowwise, FromWrapping> && mat,
		hbrs::mpl::matrix_distribution<
			integral_constant<El::Dist, ToColumnwise>,
			integral_constant<El::Dist, ToRowwise>,
			integral_constant<El::DistWrap, ToWrapping>
		>
	) {
		return { HBRS_MPL_FWD(mat).data() };
	}
	
	template <
		typename Ring,
		El::Dist FromColumnwise, El::Dist FromRowwise, El::DistWrap FromWrapping,
		El::Dist ToColumnwise, El::Dist ToRowwise, El::DistWrap ToWrapping
	>
	static hbrs::mpl::el_dist_matrix<Ring, ToColumnwise, ToRowwise, ToWrapping>
	apply(
		hbrs::mpl::el_dist_matrix<Ring, FromColumnwise, FromRowwise, FromWrapping> const& mat,
		hbrs::mpl::matrix_distribution<
			integral_constant<El::Dist, ToColumnwise>,
			integral_constant<El::Dist, ToRowwise>,
			integral_constant<El::DistWrap, ToWrapping>
		>
	) {
		return { mat.data() };
	}
};

BOOST_HANA_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_DT_EL_DIST_MATRIX_IMPL_HPP
