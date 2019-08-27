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

#ifndef HBRS_MPL_DT_GIVENS_ROTATION_IMPL_HPP
#define HBRS_MPL_DT_GIVENS_ROTATION_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <array>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/type.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template<typename /* type of matrix entries */ Ring>
struct givens_rotation {
	givens_rotation(std::size_t const i, std::size_t const k, std::array<Ring, 2> const theta) : i_{i}, k_{k}, theta_{theta} {}

	decltype(auto)
	i() const {
		return i_;
	}

	decltype(auto)
	k() const {
		return k_;
	}

	decltype(auto)
	theta() const {
		return (theta_);
	}
private:
	std::size_t i_, k_;
	std::array<Ring, 2> theta_;
};

template<typename Ring>
auto
G(std::size_t const i, std::size_t const k, std::array<Ring, 2> const theta) {
	return givens_rotation{i,k,theta};
}

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Ring>
struct tag_of< hbrs::mpl::givens_rotation<Ring> > {
	using type = hbrs::mpl::givens_rotation_tag;
};

template <>
struct make_impl<hbrs::mpl::givens_rotation_tag> {
	template <typename Ring>
	static hbrs::mpl::givens_rotation<Ring>
	apply(hana::basic_type<Ring>, std::size_t i, std::size_t k, std::array<Ring, 2> theta) {
		return {i,k,theta};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_GIVENS_ROTATION_IMPL_HPP