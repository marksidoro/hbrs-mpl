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

#ifndef HBRS_MPL_DT_DMD_RESULT_IMPL_HPP
#define HBRS_MPL_DT_DMD_RESULT_IMPL_HPP

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename Eigenvalues, typename Modes>
struct dmd_result {
	template<
		typename Eigenvalues_ = Eigenvalues,
		typename Modes_ = Modes,
		typename std::enable_if_t<
			std::is_default_constructible<Eigenvalues_>::value &&
			std::is_default_constructible<Modes_>::value
		>* = nullptr
	>
	constexpr
	dmd_result() {}
	
	template<
		typename Eigenvalues_ = Eigenvalues,
		typename Modes_ = Modes,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<Eigenvalues, Eigenvalues_> &&
			detail::is_braces_constructible_v<Modes, Modes_>
		>* = nullptr
	>
	constexpr 
	dmd_result(Eigenvalues_ && eigenvalues, Modes_ && modes)
	: eigenvalues_{HBRS_MPL_FWD(eigenvalues)}, modes_{HBRS_MPL_FWD(modes)}
	{}
	
	constexpr 
	dmd_result(dmd_result const&) = default;
	constexpr 
	dmd_result(dmd_result &&) = default;
	
	constexpr dmd_result&
	operator=(dmd_result const&) = default;
	constexpr dmd_result&
	operator=(dmd_result &&) = default;
	
	constexpr decltype(auto)
	eigenvalues() & { return (eigenvalues_); };
	
	constexpr decltype(auto)
	eigenvalues() const& { return (eigenvalues_); };
	
	constexpr decltype(auto)
	eigenvalues() && { return HBRS_MPL_FWD(eigenvalues_); };
	
	constexpr decltype(auto)
	modes() & { return (modes_); };
	
	constexpr decltype(auto)
	modes() const& { return (modes_); };
	
	constexpr decltype(auto)
	modes() && { return HBRS_MPL_FWD(modes_); };
	
private:
	Eigenvalues eigenvalues_;
	Modes modes_;
};

struct dmd_eigenvalues{};
struct dmd_modes{};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Eigenvalues, typename Modes>
struct tag_of< hbrs::mpl::dmd_result<Eigenvalues, Modes> > {
	using type = hbrs::mpl::dmd_result_tag;
};

template <>
struct make_impl<hbrs::mpl::dmd_result_tag> {
	template <typename Eigenvalues, typename Modes>
	static constexpr hbrs::mpl::dmd_result<
		std::decay_t<Eigenvalues>,
		std::decay_t<Modes>
	>
	apply(Eigenvalues && eigenvalues, Modes && modes) {
		return {
			HBRS_MPL_FWD(eigenvalues),
			HBRS_MPL_FWD(modes)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_DMD_RESULT_IMPL_HPP
