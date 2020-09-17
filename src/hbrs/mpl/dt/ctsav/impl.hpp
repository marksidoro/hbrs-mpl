/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_CTSAV_IMPL_HPP
#define HBRS_MPL_DT_CTSAV_IMPL_HPP
/* compile-time-size array view */

#include "fwd.hpp"
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <typename T, std::size_t Length>
struct ctsav {
public:
	
	template<std::size_t N>
	constexpr
	ctsav(T (&a)[N]) noexcept
	: data_{ a }
	{
		static_assert(N > 0, "Zero-length array is not permitted in ISO C++.");
		static_assert(Length <= N, "View length must be equal or less than array length.");
	}
	
	constexpr
	ctsav(ctsav const&) noexcept = default;
	constexpr
	ctsav(ctsav &&) noexcept = default;
	
	constexpr ctsav&
	operator=(ctsav const&) noexcept = default;
	constexpr ctsav&
	operator=(ctsav &&) noexcept = default;
	
	constexpr decltype(auto)
	begin() noexcept {
		return data_;
	}
	
	constexpr decltype(auto)
	begin() const noexcept {
		return data_;
	}
	
	constexpr auto
	end() noexcept {
		return data_ + Length;
	}
	
	constexpr auto 
	end() const noexcept {
		return data_ + Length;
	}
	
	constexpr auto
	length() const noexcept {
		return Length;
	}
	
	constexpr decltype(auto)
	operator[](std::size_t i) noexcept {
		//static_assert(0 <= i && i < Length, "TODO");
		return data_[i];
	}
	
	constexpr decltype(auto)
	operator[](std::size_t i) const noexcept {
		//static_assert(0 <= i && i < Length, "TODO");
		return data_[i];
	}
	
private:
	T (&data_) [Length];
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename T, std::size_t Length>
struct tag_of< hbrs::mpl::ctsav<T, Length> > {
	using type = hbrs::mpl::ctsav_tag;
};

template <>
struct make_impl<hbrs::mpl::ctsav_tag> {
	template <typename T, std::size_t Length>
	static constexpr hbrs::mpl::ctsav<T, Length>
	apply(T (&a)[Length]) {
		return {HBRS_MPL_FWD(a)};
	}
};

/* namespace hana */ } /* namespace boost */ }

HBRS_MPL_NAMESPACE_BEGIN
extern template struct HBRS_MPL_API ctsav<int, 1>;
extern template struct HBRS_MPL_API ctsav<int, 2>;
extern template struct HBRS_MPL_API ctsav<int, 3>;
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DT_CTSAV_IMPL_HPP
