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

#ifndef HBRS_MPL_DT_SMRS_IMPL_HPP
#define HBRS_MPL_DT_SMRS_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/detail/is_braces_constructible.hpp>

#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/size.hpp>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename Matrix>
struct smrs {
	template<
		typename Matrix_,
		typename std::enable_if_t<
			detail::is_braces_constructible<Matrix, Matrix_&&>::value &&
			!std::is_base_of<smrs, std::remove_reference_t<Matrix_> >::value
		>* = nullptr
	>
	constexpr
	smrs(Matrix_ && a) : a_{HBRS_MPL_FWD(a)} {}

	constexpr
	smrs(smrs const&) = default;
	constexpr
	smrs(smrs &&) = default;
	
	constexpr smrs&
	operator=(smrs const&) = default;
	constexpr smrs&
	operator=(smrs &&) = default;

	template<typename Index>
	constexpr decltype(auto)
	at(Index && row) & {
		return smr<std::add_lvalue_reference_t<Matrix>, std::decay_t<Index>>{ a_, HBRS_MPL_FWD(row) };
	}
	
	template<typename Index>
	constexpr decltype(auto)
	at(Index && row) const& {
		return smr<std::add_lvalue_reference_t<const Matrix>, std::decay_t<Index>>{ a_, HBRS_MPL_FWD(row) };
	}
	
	template<typename Index>
	constexpr decltype(auto)
	at(Index && row) && {
		/* NOTE: Make sure to call smrs::at&& only once on a temporary smcs, 
		 * because a_ will have be moved away once smrs::at&& has been called!
		 */
		return make_smr(std::move(a_), HBRS_MPL_FWD(row));
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
	
	constexpr decltype(auto)
	data() & {
		return (a_);
	}
	
	constexpr decltype(auto)
	data() const& {
		return (a_);
	}
	
	constexpr decltype(auto)
	data() && {
		return HBRS_MPL_FWD(a_);
	}
private:
	Matrix a_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Matrix>
struct tag_of< hbrs::mpl::smrs<Matrix> > {
	using type = hbrs::mpl::smrs_tag;
};

/* namespace hana */ } /* namespace boost */ }

namespace boost { namespace hana {

template <>
struct make_impl<hbrs::mpl::smrs_tag> {
	template<typename Matrix>
	static constexpr hbrs::mpl::smrs< std::decay_t<Matrix> >
	apply(Matrix && a) {
		return {HBRS_MPL_FWD(a)};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_SMRS_IMPL_HPP
