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

#ifndef HBRS_MPL_DT_SMS_HPP
#define HBRS_MPL_DT_SMS_HPP

#include <hbrs/mpl/dt/sms/fwd.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/matrix_index.hpp>

#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/div.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/mult.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

struct column_major_accessor {
	template<typename Matrix, typename Index1D>
	constexpr decltype(auto)
	operator()(Matrix && a, Index1D const& i) const {
		auto sz = (*size)(a);
		auto n_ = i % (*m)(sz);
		auto m_ = i - n_ * (*m)(sz);
		return (*at)(HBRS_MPL_FWD(a), make_matrix_index(m_, n_));
	}
};

struct row_major_accessor {
	template<typename Matrix, typename Index1D>
	constexpr decltype(auto)
	operator()(Matrix && a, Index1D const& i) const {
		auto sz = (*size)(a);
		auto m_ = i % (*n)(sz);
		auto n_ = i - m_ * (*n)(sz);
		return (*at)(HBRS_MPL_FWD(a), make_matrix_index(m_, n_));
	}
};

template<
	typename Matrix, 
	typename Accessor
>
struct sms {
	
	template<typename Matrix_, typename Accessor_>
	constexpr 
	sms(Matrix_ && mat, Accessor_ && acc)
	: mat_{HBRS_MPL_FWD(mat)}, acc_{HBRS_MPL_FWD(acc)}
	{}
	
	constexpr 
	sms(sms const&) = default;
	constexpr 
	sms(sms &&) = default;
	
	constexpr sms&
	operator=(sms const&) = default;
	constexpr sms&
	operator=(sms &&) = default;
	
	constexpr auto
	length() const { 
		auto sz = (*size)(mat_);
		auto m_ = (*m)(sz);
		auto n_ = (*n)(sz);
		return m_ * n_;
	}
	
	template<typename Index>
	constexpr auto &
	at(Index && i) { return acc_(mat_, HBRS_MPL_FWD(i)); }
	
	template<typename Index>
	constexpr auto const&
	at(Index && i) const { return acc_(mat_, HBRS_MPL_FWD(i)); }
	
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) & { return at(HBRS_MPL_FWD(i)); }
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) const& { return at(HBRS_MPL_FWD(i)); }
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) && { return at(HBRS_MPL_FWD(i)); }
	
private:
	Matrix mat_;
	Accessor acc_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <
	typename Matrix, 
	typename Accessor
>
struct tag_of< hbrs::mpl::sms<Matrix, Accessor> > {
	using type = hbrs::mpl::sms_tag;
};

template <>
struct make_impl<hbrs::mpl::sms_tag> {
	
	template <
		typename Matrix, 
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, hbrs::mpl::ctsam_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, hbrs::mpl::rtsam_tag >::value ||
			std::is_same< hana::tag_of_t<Matrix>, hbrs::mpl::sm_tag >::value
		>* = nullptr
	>
	static constexpr decltype(auto)
	apply(Matrix && m) {
		return helper(
			HBRS_MPL_FWD(m),
			std::decay_t<decltype(m.order())>{}
		);
	}
	
private:
	template<typename Matrix>
	static constexpr hbrs::mpl::sms<
		std::decay_t<Matrix>,
		hbrs::mpl::row_major_accessor
	>
	helper(
		Matrix && m, 
		hbrs::mpl::storage_order_<
			hbrs::mpl::storage_order::row_major
		>
	) {
		return {
			HBRS_MPL_FWD(m),
			hbrs::mpl::row_major_accessor{}
		};
	}
	
	template<typename Matrix>
	static constexpr hbrs::mpl::sms<
		std::decay_t<Matrix>,
		hbrs::mpl::column_major_accessor
	>
	helper(
		Matrix && m, 
		hbrs::mpl::storage_order_<
			hbrs::mpl::storage_order::column_major
		>
	) {
		return {
			HBRS_MPL_FWD(m),
			hbrs::mpl::column_major_accessor{}
		};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_SMS_HPP
