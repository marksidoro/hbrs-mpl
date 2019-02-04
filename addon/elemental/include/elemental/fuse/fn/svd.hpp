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

#pragma once

#ifndef ELEMENTAL_FUSE_FN_SVD_HPP
#define ELEMENTAL_FUSE_FN_SVD_HPP

#include <elemental/config.hpp>
#include <elemental/detail/Ring.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>
#include <hbrs/mpl/dt/decompose_mode.hpp>
#include <hbrs/mpl/dt/svd_result.hpp>
#include <boost/assert.hpp>
#include <El.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;
namespace detail {

/* Example: https://github.com/elemental/Elemental/blob/master/tests/lapack_like/SVD.cpp */
template <typename A, typename U, typename S, typename V>
auto
svd_impl(A const& a, mpl::decompose_mode mode, U u, S s, S s_, V v) {
	typedef Ring_t<std::decay_t<A>> Ring;
	typedef std::decay_t<Ring> _Ring_;
	
	El::SVDCtrl<El::Base<_Ring_>> ctrl;
	ctrl.useLAPACK = true;
	ctrl.useScaLAPACK = true;
	ctrl.bidiagSVDCtrl.wantU = true;
	ctrl.bidiagSVDCtrl.wantV = true;
	
	if (mode == mpl::decompose_mode::complete) {
		ctrl.bidiagSVDCtrl.approach = El::SVDApproach::FULL_SVD;
	} else if (mode == mpl::decompose_mode::economy) {
		ctrl.bidiagSVDCtrl.approach = El::SVDApproach::THIN_SVD;
	} else if (mode == mpl::decompose_mode::zero) {
		if (a.Height() > a.Width()) {
			ctrl.bidiagSVDCtrl.approach = El::SVDApproach::THIN_SVD;
		} else {
			ctrl.bidiagSVDCtrl.approach = El::SVDApproach::FULL_SVD;
		}
	}
	
	/* auto info = */El::SVD(a, u, s, v, ctrl);
	BOOST_ASSERT(!u.Locked());
	BOOST_ASSERT(!s.Locked());
	BOOST_ASSERT(!v.Locked());
	
	if (mode == mpl::decompose_mode::complete) {
		El::Zeros(s_, a.Height(), a.Width());
	} else if (mode == mpl::decompose_mode::economy) {
		// copy singular values from n*1 matrix to diagonal of n*n matrix
		auto min_mn = std::min(a.Height(), a.Width());
		BOOST_ASSERT(min_mn >= s.Height());
		El::Zeros(s_, min_mn, min_mn);
	} else if (mode == mpl::decompose_mode::zero) {
		if (a.Height() > a.Width()) {
			auto min_mn = std::min(a.Height(), a.Width());
			BOOST_ASSERT(min_mn >= s.Height());
			El::Zeros(s_, min_mn, min_mn);
		} else {
			El::Zeros(s_, a.Height(), a.Width());
		}
	}
	
	//TODO: make abstraction instead of copying values?
	for(El::Int i = 0; i < s.Height(); ++i) {
		s_.Set(i,i, s.Get(i,0));
	}
	
	if (ctrl.bidiagSVDCtrl.approach == El::SVDApproach::THIN_SVD) {
		// u is m x min(m,n)
		BOOST_ASSERT(u.Height() == a.Height());
		BOOST_ASSERT(u.Width() == std::min(a.Height(), a.Width()));
		
		// v is n x min(m,n)
		BOOST_ASSERT(v.Height() == a.Width());
		BOOST_ASSERT(v.Width() == std::min(a.Height(), a.Width()));
	}
	
	return mpl::make_svd_result(u, s_, v);
}

struct svd_impl_Matrix {
	template <typename Field>
	auto
	operator()(El::Matrix<Field> const& a, mpl::decompose_mode mode) const {
		typedef std::decay_t<Field> _Field_;
		return svd_impl(
			a, mode,
			El::Matrix<_Field_>{} /*u*/,
			El::Matrix<El::Base<_Field_>>{} /*s*/,
			El::Matrix<El::Base<_Field_>>{} /*s_*/,
			El::Matrix<_Field_>{} /*v*/
		);
	}
};

struct svd_impl_AbstractDistMatrix {
	template <typename Field>
	auto
	operator()(El::AbstractDistMatrix<Field> const& a, mpl::decompose_mode mode) const {
		typedef std::decay_t<Field> _Field_;
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
		return svd_impl(
			a, mode,
			El::DistMatrix<_Field_, El::MC, El::MR>{a.Grid()} /*u*/,
			El::DistMatrix<El::Base<_Field_>, El::STAR, El::STAR>{a.Grid()} /*s*/,
			El::DistMatrix<El::Base<_Field_>, El::STAR, El::STAR>{a.Grid()} /*s_*/,
			El::DistMatrix<_Field_, El::MC, El::MR>{a.Grid()} /*v*/
		);
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_SVD_IMPLS boost::hana::make_tuple(                                                           \
		elemental::detail::svd_impl_Matrix{},                                                                          \
		elemental::detail::svd_impl_AbstractDistMatrix{}                                                               \
	)

#endif // !ELEMENTAL_FUSE_FN_SVD_HPP
