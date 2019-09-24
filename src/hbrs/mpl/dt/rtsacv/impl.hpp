/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_RTSACV_IMPL_HPP
#define HBRS_MPL_DT_RTSACV_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <vector>
#include <type_traits>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/type.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template<typename /* type of vector entries */ Ring>
//TODO: Merge with rtsarv
struct rtsacv {
	rtsacv(std::vector<Ring> data) : data_{data} {}
	
	explicit rtsacv(std::size_t size) : data_(size, Ring{0}) {}

	rtsacv(rtsacv const&) = default;
	rtsacv(rtsacv &&) = default;
	
	rtsacv&
	operator=(rtsacv const&) = default;
	rtsacv&
	operator=(rtsacv &&) = default;
	
	auto
	length() const {
		return data_.size();
	}

	decltype(auto)
	at(std::size_t i) {
		return data_.at(i);
	}
	
	decltype(auto)
	at(std::size_t i) const {
		return data_.at(i);
	}
	
	decltype(auto)
	data() & { return (data_); };

	decltype(auto)
	data() const& { return (data_); };

	decltype(auto)
	data() && { return HBRS_MPL_FWD(data_); };
private:
	std::vector<Ring> data_;
};

template<typename Ring>
decltype(auto)
operator*(Ring const& s, rtsacv<Ring> const& v) {
	return (*multiply)(v,s);
}

template<
	typename LHS,
	typename RHS,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<LHS>, submatrix_tag > && std::is_same_v< hana::tag_of_t<RHS>, rtsacv_tag >
	>* = nullptr
>
decltype(auto)
operator*(LHS && lhs, RHS && rhs) {
	return (*multiply)(HBRS_MPL_FWD(lhs), HBRS_MPL_FWD(rhs));
}

template<typename Ring>
decltype(auto)
operator/(rtsacv<Ring> const& v, Ring const& d) {
	return (*divide)(v,d);
}

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Ring>
struct tag_of< hbrs::mpl::rtsacv<Ring> > {
	using type = hbrs::mpl::rtsacv_tag;
};

template <>
struct make_impl<hbrs::mpl::rtsacv_tag> {
	template <typename Ring>
	static hbrs::mpl::rtsacv<Ring>
	apply(hana::basic_type<Ring>, std::size_t size) {
		return {size};
	}
	
	template <
		typename Ring,
		typename std::enable_if_t<
			!std::is_const_v< std::remove_reference_t<Ring> >
		>* = nullptr
	>
	static hbrs::mpl::rtsacv<Ring>
	apply(std::vector<Ring> data) {
		return {data};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_RTSACV_IMPL_HPP
