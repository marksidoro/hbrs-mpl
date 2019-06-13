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

#ifndef HBRS_MPL_DT_RTSAV_IMPL_HPP
#define HBRS_MPL_DT_RTSAV_IMPL_HPP
/* runtime-size array view */

#include <hbrs/mpl/dt/rtsav/fwd.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/assert.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <typename T>
struct rtsav {
public:
	
	constexpr
	rtsav() noexcept
	: data_{nullptr}, length_{0}
	{}
	
	/* This is a reference to a pointer to clarify that rtsav's lifetime depends on lifetime of array! */
	constexpr
	rtsav(T*& a, std::size_t length) noexcept
	: data_{ a /* length > 0 ? a : nullptr */ }, length_{length}
	{}
	
	/* This is a reference to a pointer to clarify that rtsav's lifetime depends on lifetime of array! */
	constexpr
	rtsav(T* const& a, std::size_t length) noexcept
	: data_{ a /* length > 0 ? a : nullptr */ }, length_{length}
	{}
	
	constexpr
	rtsav(rtsav const&) noexcept = default;
	
	constexpr
	rtsav(rtsav &&) noexcept = default;
	
	constexpr rtsav& 
	operator=(rtsav const&) noexcept = default;
	constexpr rtsav& 
	operator=(rtsav &&) noexcept = default;
	
	
	constexpr decltype(auto)
	begin() noexcept {
		return data_;
	}
	
	constexpr decltype(auto)
	begin() const noexcept {
		return data_;
	}
	
	constexpr decltype(auto)
	end() noexcept {
		return data_ + length_;
	}
	
	constexpr decltype(auto) 
	end() const noexcept {
		return data_ + length_;
	}
	
	constexpr auto 
	length() const noexcept {
		return length_;
	}
	
	constexpr decltype(auto)
	operator[](std::size_t i) noexcept {
		BOOST_ASSERT(0 <= i && i < length_);
		return data_[i];
	}
	
	constexpr decltype(auto)
	operator[](std::size_t i) const noexcept {
		BOOST_ASSERT(0 <= i && i < length_);
		return data_[i];
	}
	
	constexpr auto
	data() noexcept {
		return data_;
	}
	
	constexpr auto
	data() const noexcept {
		return data_;
	}
	
private:
	T* data_;
	std::size_t const length_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename T>
struct tag_of< hbrs::mpl::rtsav<T> > {
	using type = hbrs::mpl::rtsav_tag;
};

template <>
struct make_impl<hbrs::mpl::rtsav_tag> {
	template <typename T>
	static constexpr hbrs::mpl::rtsav<T>
	apply(T* & a, std::size_t l) {
		return {a, l};
	}
	
	template <typename T>
	static constexpr hbrs::mpl::rtsav<T>
	apply(T* const& a, std::size_t l) {
		return {a, l};
	}
	
	template <typename T, std::size_t Length>
	static constexpr hbrs::mpl::rtsav<T>
	apply(T (&a) [Length]) {
		return {a, Length};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_RTSAV_IMPL_HPP
