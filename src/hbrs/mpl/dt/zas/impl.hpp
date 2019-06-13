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

#ifndef HBRS_MPL_DT_ZAS_IMPL_HPP
#define HBRS_MPL_DT_ZAS_IMPL_HPP

#include <hbrs/mpl/dt/zas/fwd.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/equal.hpp>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>

#include <boost/hana/pair.hpp>
#include <boost/assert.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename SequenceA, typename SequenceB>
struct zas {
	template<typename SequenceA_, typename SequenceB_>
	constexpr
	zas(SequenceA_ && a, SequenceB_ && b) : a_{HBRS_MPL_FWD(a)}, b_{HBRS_MPL_FWD(b)} {
		//NOTE: a and b might have been moved to a_ and b_, so do not access them in ctor
		BOOST_ASSERT((*equal)((*size)(a_), (*size)(b_)));
	}

	constexpr
	zas(zas const&) = default;
	constexpr
	zas(zas &&) = default;
	
	constexpr zas&
	operator=(zas const&) = default;
	constexpr zas&
	operator=(zas &&) = default;

	template<typename Index>
	constexpr auto
	at(Index && i) {
		using hbrs::mpl::at;
		return make_lvalue_pair((*at)(a_, i), (*at)(b_, i));
	}

	template<typename Index>
	constexpr auto
	at(Index && i) const {
		using hbrs::mpl::at;
		return make_lvalue_pair((*at)(a_, i), (*at)(b_, i));
	}
	
	constexpr auto
	length() const {
		return (*mpl::size)(a_);
	}
	
	template<typename Index>
	constexpr decltype(auto)
	operator[](Index && i) {
		return at(HBRS_MPL_FWD(i));
	}
	
	template<typename Index>
	constexpr decltype(auto)
	operator[](Index && i) const {
		return at(HBRS_MPL_FWD(i));
	}

private:
	template<typename A, typename B>
	static constexpr auto
	make_lvalue_pair(A && a, B && b) {
		return hana::pair<
			std::conditional_t<
				std::is_lvalue_reference<A>::value,
				A,
				std::decay_t<A>
			>,
			std::conditional_t<
				std::is_lvalue_reference<B>::value,
				B,
				std::decay_t<B>
			>
		>{ HBRS_MPL_FWD(a), HBRS_MPL_FWD(b) };
	}
	
	
	SequenceA a_;
	SequenceB b_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename SequenceA, typename SequenceB>
struct tag_of< hbrs::mpl::zas<SequenceA, SequenceB> > {
	using type = hbrs::mpl::zas_tag;
};

/* namespace hana */ } /* namespace boost */ }

namespace boost { namespace hana {

template <>
struct make_impl<hbrs::mpl::zas_tag> {
	template<typename SequenceA, typename SequenceB>
	static constexpr hbrs::mpl::zas< std::decay_t<SequenceA>, std::decay_t<SequenceB> >
	apply(SequenceA && a, SequenceB && b) {
		return {HBRS_MPL_FWD(a), HBRS_MPL_FWD(b)};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_ZAS_IMPL_HPP
