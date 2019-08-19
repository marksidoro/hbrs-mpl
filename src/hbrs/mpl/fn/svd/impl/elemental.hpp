/* Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_SVD_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_SVD_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/svd_control.hpp>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <hbrs/mpl/dt/svd_result.hpp>
#include <boost/assert.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

/* Example: https://github.com/elemental/Elemental/blob/master/tests/lapack_like/SVD.cpp */
template <typename A, typename U, typename S, typename V>
auto
svd_impl_el(A const& a, svd_control<decompose_mode> const& ctrl, U u, S s, S s_, V v) {
	typedef decltype(a.data().Get(0,0)) Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	El::SVDCtrl<El::Base<_Ring_>> el_ctrl;
	el_ctrl.useLAPACK = true;
	el_ctrl.useScaLAPACK = true;
	el_ctrl.bidiagSVDCtrl.wantU = true;
	el_ctrl.bidiagSVDCtrl.wantV = true;
	
	if (ctrl.decompose_mode() == decompose_mode::complete) {
		el_ctrl.bidiagSVDCtrl.approach = El::SVDApproach::FULL_SVD;
	} else if (ctrl.decompose_mode() == decompose_mode::economy) {
		el_ctrl.bidiagSVDCtrl.approach = El::SVDApproach::THIN_SVD;
	} else if (ctrl.decompose_mode() == decompose_mode::zero) {
		if (a.m() > a.n()) {
			el_ctrl.bidiagSVDCtrl.approach = El::SVDApproach::THIN_SVD;
		} else {
			el_ctrl.bidiagSVDCtrl.approach = El::SVDApproach::FULL_SVD;
		}
	}
	
	/* auto info = */El::SVD(a.data(), u, s, v, el_ctrl);
	BOOST_ASSERT(!u.Locked());
	BOOST_ASSERT(!s.Locked());
	BOOST_ASSERT(!v.Locked());
	
	if (ctrl.decompose_mode() == decompose_mode::complete) {
		El::Zeros(s_, a.m(), a.n());
	} else if (ctrl.decompose_mode() == decompose_mode::economy) {
		// copy singular values from n*1 matrix to diagonal of n*n matrix
		auto min_mn = std::min(a.m(), a.n());
		BOOST_ASSERT(min_mn >= s.Height());
		El::Zeros(s_, min_mn, min_mn);
	} else if (ctrl.decompose_mode() == decompose_mode::zero) {
		if (a.m() > a.n()) {
			auto min_mn = std::min(a.m(), a.n());
			BOOST_ASSERT(min_mn >= s.Height());
			El::Zeros(s_, min_mn, min_mn);
		} else {
			El::Zeros(s_, a.m(), a.n());
		}
	}
	
	//TODO: make abstraction instead of copying values?
	for(El::Int i = 0; i < s.Height(); ++i) {
		s_.Set(i,i, s.Get(i,0));
	}
	
	if (el_ctrl.bidiagSVDCtrl.approach == El::SVDApproach::THIN_SVD) {
		// u is m x min(m,n)
		BOOST_ASSERT(u.Height() == a.m());
		BOOST_ASSERT(u.Width() == std::min(a.m(), a.n()));
		
		// v is n x min(m,n)
		BOOST_ASSERT(v.Height() == a.n());
		BOOST_ASSERT(v.Width() == std::min(a.m(), a.n()));
	}
	
	return make_svd_result(
		hana::make<hana::tag_of_t<A>>(std::move(u)),
		hana::make<hana::tag_of_t<A>>(std::move(s_)),
		hana::make<hana::tag_of_t<A>>(std::move(v))
	);
}


template <typename Ring>
auto
svd_impl_el_matrix::operator()(
	el_matrix<Ring> const& a,
	svd_control<decompose_mode> const& ctrl
) const {
	typedef std::decay_t<Ring> _Ring_;
	return svd_impl_el(
		a, ctrl,
		El::Matrix<_Ring_>{} /*u*/,
		El::Matrix<El::Base<_Ring_>>{} /*s*/,
		El::Matrix<El::Base<_Ring_>>{} /*s_*/,
		El::Matrix<_Ring_>{} /*v*/
	);
}

template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
auto
svd_impl_el_dist_matrix::operator()(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	svd_control<decompose_mode> const& ctrl
) const {
	typedef std::decay_t<Ring> _Ring_;
	/* Elemental uses Read-/Write-Proxies to map input matrix distributions (defined by user) to matrix distribution
		* types that are suitable for SVD. Elemental uses these distributions:
		*  - with ScaLAPACK: DistMatrix<Field,MC,MR,BLOCK> for A, U and V,
		*                    DistMatrix<Real,STAR,STAR> for S,
		*                    see Elemental/src/lapack_like/spectral/SVD.cpp
		*  - without ScaLAPACK: DistMatrix<Field,MC,MR> for A, U and V,
		*                       DistMatrix<Real,STAR,STAR> for S,
		*                       see Elemental/src/lapack_like/spectral/BidiagSVD.cpp
		* To prevent unnecessary conversations or redistributions we just use those SVD distributions directly.
		* TODO: Do proxies cause redistributions or performance penalties?
		*/
	return svd_impl_el(
		a, ctrl,
		El::DistMatrix<_Ring_, El::MC, El::MR>{a.data().Grid()} /*u*/,
		El::DistMatrix<El::Base<_Ring_>, El::STAR, El::STAR>{a.data().Grid()} /*s*/,
		El::DistMatrix<El::Base<_Ring_>, El::STAR, El::STAR>{a.data().Grid()} /*s_*/,
		El::DistMatrix<_Ring_, El::MC, El::MR>{a.data().Grid()} /*v*/
	);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_SVD_IMPL_ELEMENTAL_HPP
