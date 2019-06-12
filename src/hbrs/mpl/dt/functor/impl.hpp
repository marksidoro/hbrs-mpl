/* Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DT_FUNCTOR_HPP
#define HBRS_MPL_DT_FUNCTOR_HPP

#include <hbrs/mpl/fwd/dt/functor.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

template <typename F>
struct functor {
public:
	
	template<typename F_>
	constexpr
	functor(F_ && f) : f_{HBRS_MPL_FWD(f)} {}
	
	//TODO: Implement more constructors and assignment operators?
	
	template <typename... Args>
	constexpr decltype(auto)
	operator()(Args&&... args) const {
		return f_(HBRS_MPL_FWD(args)...);
	}
	
	template <typename... Args>
	constexpr decltype(auto)
	operator()(Args&&... args) {
		return f_(HBRS_MPL_FWD(args)...);
	}
	
	constexpr decltype(auto)
	f() & { return (f_); }
	
	constexpr decltype(auto)
	f() const& { return (f_); }
	
	constexpr decltype(auto)
	f() && { return HBRS_MPL_FWD(f_); }
	
private:
	F f_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename F>
struct tag_of< hbrs::mpl::functor<F> > {
	using type = hbrs::mpl::functor_tag;
};

template <>
struct make_impl<hbrs::mpl::functor_tag> {
	template <typename F>
	static constexpr hbrs::mpl::functor<F>
	apply(F&& f) {
		return {HBRS_MPL_FWD(f)};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_FUNCTOR_HPP
