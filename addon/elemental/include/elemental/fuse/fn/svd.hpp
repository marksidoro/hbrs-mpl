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

struct svd_impl_Matrix {
	template <typename Ring>
	auto
	operator()(El::Matrix<Ring> const& A, mpl::decompose_mode mode) const {
		/* Example: https://github.com/elemental/Elemental/blob/master/tests/lapack_like/SVD.cpp */
		typedef std::decay_t<Ring> _Ring_;
		typedef El::Base<_Ring_> Real;
		
		El::SVDCtrl<Real> ctrl;
		ctrl.useLAPACK = true;
		ctrl.useScaLAPACK = true;
		ctrl.bidiagSVDCtrl.wantU = true;
		ctrl.bidiagSVDCtrl.wantV = true;
		
		if (mode == mpl::decompose_mode::complete) {
			ctrl.bidiagSVDCtrl.approach = El::SVDApproach::FULL_SVD;
		} else if (mode == mpl::decompose_mode::economy) {
			ctrl.bidiagSVDCtrl.approach = El::SVDApproach::THIN_SVD;
		} else if (mode == mpl::decompose_mode::zero) {
			if (A.Height() > A.Width()) {
				ctrl.bidiagSVDCtrl.approach = El::SVDApproach::THIN_SVD;
			} else {
				ctrl.bidiagSVDCtrl.approach = El::SVDApproach::FULL_SVD;
			}
		}
		
		El::Matrix<Real> s;
		El::Matrix<_Ring_> U, V;
		
		/* auto info = */El::SVD(A, U, s, V, ctrl);
		
		El::Matrix<Real> s_;
		if (mode == mpl::decompose_mode::complete) {
			El::Zeros(s_, A.Height(), A.Width());
		} else if (mode == mpl::decompose_mode::economy) {
			// copy singular values from n*1 matrix to diagonal of n*n matrix
			auto min_mn = std::min(A.Height(), A.Width());
			BOOST_ASSERT(min_mn >= s.Height());
			El::Zeros(s_, min_mn, min_mn);
		} else if (mode == mpl::decompose_mode::zero) {
			if (A.Height() > A.Width()) {
				auto min_mn = std::min(A.Height(), A.Width());
				BOOST_ASSERT(min_mn >= s.Height());
				El::Zeros(s_, min_mn, min_mn);
			} else {
				El::Zeros(s_, A.Height(), A.Width());
			}
		}
		
		//TODO: make abstraction instead of copying values?
		for(El::Int i = 0; i < s.Height(); ++i) {
			s_.Set(i,i, s.Get(i,0));
		}
		
		if (ctrl.bidiagSVDCtrl.approach == El::SVDApproach::THIN_SVD) {
			// U is m x min(m,n)
			BOOST_ASSERT(U.Height() == A.Height());
			BOOST_ASSERT(U.Width() == std::min(A.Height(), A.Width()));
			
			// V is n x min(m,n)
			BOOST_ASSERT(V.Height() == A.Width());
			BOOST_ASSERT(V.Width() == std::min(A.Height(), A.Width()));
		}
		
		return mpl::make_svd_result(U, s_, V);
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_SVD_IMPLS boost::hana::make_tuple(                                                           \
		elemental::detail::svd_impl_Matrix{}                                                                           \
	)

#endif // !ELEMENTAL_FUSE_FN_SVD_HPP
