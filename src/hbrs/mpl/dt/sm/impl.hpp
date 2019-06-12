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

#ifndef HBRS_MPL_DT_SM_HPP
#define HBRS_MPL_DT_SM_HPP

#include <hbrs/mpl/dt/sm/fwd.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/detail/translate_index.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/equal.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <boost/assert.hpp>

HBRS_MPL_NAMESPACE_BEGIN

template<
	typename Sequence, 
	typename /* Matrix */ Size,
	storage_order Order
>
struct sm {
	
	template<typename Sequence_, typename Size_>
	constexpr 
	sm(Sequence_ && seq, Size_ && size)
	: seq_{HBRS_MPL_FWD(seq)}, size_{HBRS_MPL_FWD(size)}
	{
		BOOST_ASSERT(
			(*equal)(
				hbrs::mpl::size(seq_), 
				multiply(m(size_), n(size_))
			)
		);
	}
	
	constexpr 
	sm(sm const&) = default;
	constexpr 
	sm(sm &&) = default;
	
	constexpr sm&
	operator=(sm const&) = default;
	constexpr sm&
	operator=(sm &&) = default;
	
	constexpr decltype(auto)
	size() const { return (size_); };
	
	constexpr decltype(auto)
	order() const { return storage_order_c<Order>; }
	
	template<typename Index>
	constexpr decltype(auto)
	at(Index && i) {
		return (*hbrs::mpl::at)(
			seq_,
			detail::translate_index(size_, HBRS_MPL_FWD(i), storage_order_c<Order>)
		);
	}
	
	template<typename Index>
	constexpr decltype(auto)
	at(Index && i) const {
		return (*hbrs::mpl::at)(
			seq_,
			detail::translate_index(size_, HBRS_MPL_FWD(i), storage_order_c<Order>)
		);
	}
	
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) & { return smr<sm &, std::decay_t<Index>>{*this, HBRS_MPL_FWD(i)}; }
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) const& { return smr<sm const&, std::decay_t<Index>>{*this, HBRS_MPL_FWD(i)}; }
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) && { return make_smr(std::move(*this), HBRS_MPL_FWD(i)); }
	
private:
	Sequence seq_;
	Size const size_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <
	typename Sequence, 
	typename Size,
	hbrs::mpl::storage_order Order
>
struct tag_of< hbrs::mpl::sm<Sequence, Size, Order> > {
	using type = hbrs::mpl::sm_tag;
};

template <>
struct make_impl<hbrs::mpl::sm_tag> {
	
	template <
		typename Sequence, 
		typename Size,
		hbrs::mpl::storage_order Order
	>
	static constexpr hbrs::mpl::sm< 
		std::decay_t<Sequence>,
		std::decay_t<Size>,
		Order
	>
	apply(Sequence && seq, Size && size, hbrs::mpl::storage_order_<Order>) {
		return {HBRS_MPL_FWD(seq), HBRS_MPL_FWD(size)};
	}
};

/* namespace hana */ } /* namespace boost */ }


#endif // !HBRS_MPL_DT_SM_HPP
