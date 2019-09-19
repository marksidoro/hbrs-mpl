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

#ifndef HBRS_MPL_DT_ALMOST_EQUAL_CONTROL_IMPL_HPP
#define HBRS_MPL_DT_ALMOST_EQUAL_CONTROL_IMPL_HPP

#include "fwd.hpp"

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename MaxULPsDiff, typename AlmostZeroPrecision>
struct almost_equal_control {
	template<
		typename MaxULPsDiff_ = MaxULPsDiff,
		typename AlmostZeroPrecision_ = AlmostZeroPrecision,
		typename std::enable_if_t<
			std::is_default_constructible_v<MaxULPsDiff_> &&
			std::is_default_constructible_v<AlmostZeroPrecision_>
		>* = nullptr
	>
	constexpr
	almost_equal_control() {}
	
	template<typename MaxULPsDiff_, typename AlmostZeroPrecision_>
	constexpr 
	almost_equal_control(MaxULPsDiff_ && d, AlmostZeroPrecision_ && p)
	: max_ulps_diff_{HBRS_MPL_FWD(d)}, almost_zero_precision_{HBRS_MPL_FWD(p)}
	{}
	
	constexpr 
	almost_equal_control(almost_equal_control const&) = default;
	constexpr 
	almost_equal_control(almost_equal_control &&) = default;
	
	constexpr almost_equal_control&
	operator=(almost_equal_control const&) = default;
	constexpr almost_equal_control&
	operator=(almost_equal_control &&) = default;
	
	constexpr decltype(auto)
	max_ulps_diff() & { return (max_ulps_diff_); };
	
	constexpr decltype(auto)
	max_ulps_diff() const& { return (max_ulps_diff_); };
	
	constexpr decltype(auto)
	max_ulps_diff() && { return HBRS_MPL_FWD(max_ulps_diff_); };
	
	constexpr decltype(auto)
	almost_zero_precision() & { return (almost_zero_precision_); };
	
	constexpr decltype(auto)
	almost_zero_precision() const& { return (almost_zero_precision_); };
	
	constexpr decltype(auto)
	almost_zero_precision() && { return HBRS_MPL_FWD(almost_zero_precision_); };
	
private:
	MaxULPsDiff max_ulps_diff_;
	AlmostZeroPrecision almost_zero_precision_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename MaxULPsDiff, typename AlmostZeroPrecision>
struct tag_of< hbrs::mpl::almost_equal_control<MaxULPsDiff, AlmostZeroPrecision> > {
	using type = hbrs::mpl::almost_equal_control_tag;
};

template <>
struct make_impl<hbrs::mpl::almost_equal_control_tag> {
	template <typename MaxULPsDiff, typename AlmostZeroPrecision>
	static constexpr hbrs::mpl::almost_equal_control<
		std::decay_t<MaxULPsDiff>,
		std::decay_t<AlmostZeroPrecision>
	>
	apply(MaxULPsDiff && d, AlmostZeroPrecision && p) {
		return {
			HBRS_MPL_FWD(d),
			HBRS_MPL_FWD(p)
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_ALMOST_EQUAL_CONTROL_IMPL_HPP
