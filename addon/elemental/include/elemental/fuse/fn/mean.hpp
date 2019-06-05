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

#ifndef ELEMENTAL_FUSE_FN_MEAN_HPP
#define ELEMENTAL_FUSE_FN_MEAN_HPP

#include <elemental/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>

#include <elemental/dt/matrix.hpp>
#include <elemental/dt/vector.hpp>
#include <elemental/dt/dist_matrix.hpp>
#include <elemental/dt/dist_vector.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/smc.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/sum.hpp>

#include <hbrs/mpl/dt/smcs.hpp>
#include <hbrs/mpl/dt/smrs.hpp>
#include <hbrs/mpl/fwd/fn/columns.hpp>
#include <hbrs/mpl/dt/expression.hpp>

#include <elemental/dt/exception.hpp>
#include <El.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

struct mean_impl_smcs_matrix {
	template <
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(mpl::smcs<Matrix> const& a) const {
		using namespace hbrs::mpl;
		
		auto v = (*sum)(a);
		auto a_sz = (*size)(a.data());
		auto a_m = (*m)(a_sz);
		
		El::Scale(1.0/a_m, v.data());
		return v;
	}
};

struct mean_impl_smrs_matrix {
	template <
		typename Matrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<Matrix>, matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(mpl::smrs<Matrix> const& a) const {
		using namespace hbrs::mpl;
		
		auto v = (*sum)(a);
		auto a_sz = (*size)(a.data());
		auto a_n = (*n)(a_sz);
		
		El::Scale(1.0/a_n, v.data());
		return v;
	}
};

//TODO: Replace this hack!
struct mean_impl_DistMatrix_columns {
	template <
		typename DistMatrix,
		typename std::enable_if_t<
			std::is_same< hana::tag_of_t<DistMatrix>, hana::ext::El::DistMatrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(
		mpl::expression<
			mpl::columns_t,
			std::tuple<DistMatrix>
		> const& expr
	) const {
		using namespace hbrs::mpl;
		auto const& from = hana::at_c<0>(expr.operands());
		
		typedef Ring_t<std::decay_t<DistMatrix>> Ring;
		typedef std::decay_t<Ring> _Ring_;
		
		auto sums_dmat = sum(expr).data();
		BOOST_ASSERT(sums_dmat.Height() == 1);
		BOOST_ASSERT(sums_dmat.Width() == from.Width());
		
		//TODO: Once sums_dmat is converted from "El::STAR, El::STAR" to "El::CIRC, El::CIRC" do only on sums_dmat.Grid().Rank() == 0!
		
		El::Scale(1.0/from.Height(), sums_dmat);
		
		//TODO: Adapt El::ELEMENT changed.
		return dist_row_vector<El::DistMatrix<_Ring_, El::STAR, El::STAR, El::ELEMENT>>{sums_dmat};
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_MEAN_IMPLS boost::hana::make_tuple(                                                          \
		elemental::detail::mean_impl_smcs_matrix{},                                                                    \
		elemental::detail::mean_impl_smrs_matrix{},                                                                    \
		elemental::detail::mean_impl_DistMatrix_columns{}                                                              \
	)

#endif // !ELEMENTAL_FUSE_FN_MEAN_HPP
