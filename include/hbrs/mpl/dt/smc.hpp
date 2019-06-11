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

#ifndef HBRS_MPL_DT_SMC_HPP
#define HBRS_MPL_DT_SMC_HPP

#include <hbrs/mpl/fwd/dt/smc.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/size.hpp>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template<
	typename Matrix,
	typename Index
>
struct smc {
	template<typename Matrix_, typename Index_>
	constexpr
	smc(Matrix_ && a, Index_ && column) : a_{HBRS_MPL_FWD(a)}, column_{HBRS_MPL_FWD(column)} {}

	constexpr
	smc(smc const&) = default;
	constexpr
	smc(smc &&) = default;
	
	constexpr smc&
	operator=(smc const&) = default;
	constexpr smc&
	operator=(smc &&) = default;

	
	/* Allow copy construction of a column from a column view */
	template<
		typename Matrix_,
		typename Index_,
		typename std::enable_if_t<
			!std::is_lvalue_reference<Matrix>::value &&
			std::is_lvalue_reference<Matrix_>::value
		>* = nullptr
	>
	constexpr
	smc(smc<Matrix, Index> const& rhs) : a_{rhs.a_}, column_{rhs.column_} {}

	template<typename Index_>
	constexpr decltype(auto)
	at(Index_ && row) {
		using hbrs::mpl::at;
		return (*at)(a_, make_matrix_index(HBRS_MPL_FWD(row), column_));
	}

	template<typename Index_>
	constexpr decltype(auto)
	at(Index_ && row) const {
		using hbrs::mpl::at;
		return (*at)(a_, make_matrix_index(HBRS_MPL_FWD(row), column_));
	}
	
	constexpr auto
	length() const {
		using mpl::m;
		return (*m)(size(a_));
	}
	
	template<typename Index_>
	constexpr decltype(auto)
	operator[](Index_ && row) {
		return at(HBRS_MPL_FWD(row));
	}
	
	template<typename Index_>
	constexpr decltype(auto)
	operator[](Index_ && row) const {
		return at(HBRS_MPL_FWD(row));
	}

private:
	Matrix a_;
	Index column_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Matrix, typename Index>
struct tag_of< hbrs::mpl::smc<Matrix, Index> > {
	using type = hbrs::mpl::smc_tag;
};

/* namespace hana */ } /* namespace boost */ }

namespace boost { namespace hana {

template <>
struct make_impl<hbrs::mpl::smc_tag> {
	template<typename Matrix, typename Index>
	static constexpr hbrs::mpl::smc< std::decay_t<Matrix>, std::decay_t<Index> >
	apply(Matrix && a, Index && column) {
		return {HBRS_MPL_FWD(a), HBRS_MPL_FWD(column)};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_SMC_HPP
