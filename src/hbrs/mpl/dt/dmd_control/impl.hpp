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

#ifndef HBRS_MPL_DT_DMD_CONTROL_IMPL_HPP
#define HBRS_MPL_DT_DMD_CONTROL_IMPL_HPP

#include "fwd.hpp"

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename TargetRank>
struct dmd_control {
	template<
		typename TargetRank_ = TargetRank,
		typename std::enable_if_t<
			std::is_default_constructible<TargetRank_>::value
		>* = nullptr
	>
	constexpr
	dmd_control() {}
	
	template<
		typename TargetRank_ = TargetRank,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<TargetRank, TargetRank_> &&
			!std::is_base_of_v<dmd_control, std::remove_reference_t<TargetRank_> >
		>* = nullptr
	>
	constexpr 
	dmd_control(TargetRank_ && tr)
	: target_rank_{HBRS_MPL_FWD(tr)}
	{}
	
	constexpr 
	dmd_control(dmd_control const&) = default;
	constexpr 
	dmd_control(dmd_control &&) = default;
	
	constexpr dmd_control&
	operator=(dmd_control const&) = default;
	constexpr dmd_control&
	operator=(dmd_control &&) = default;
	
	constexpr decltype(auto)
	target_rank() & { return (target_rank_); };
	
	constexpr decltype(auto)
	target_rank() const& { return (target_rank_); };
	
	constexpr decltype(auto)
	target_rank() && { return HBRS_MPL_FWD(target_rank_); };
	
private:
	TargetRank target_rank_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename TargetRank>
struct tag_of< hbrs::mpl::dmd_control<TargetRank> > {
	using type = hbrs::mpl::dmd_control_tag;
};

template <>
struct make_impl<hbrs::mpl::dmd_control_tag> {
	template <typename TargetRank>
	static constexpr hbrs::mpl::dmd_control<
		std::decay_t<TargetRank>
	>
	apply(TargetRank && tr) {
		return { HBRS_MPL_FWD(tr) };
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_DMD_CONTROL_IMPL_HPP
