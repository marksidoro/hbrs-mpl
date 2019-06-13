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

#ifndef HBRS_MPL_DT_CTSAM_IMPL_HPP
#define HBRS_MPL_DT_CTSAM_IMPL_HPP

#include <hbrs/mpl/dt/ctsam/fwd.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/detail/translate_index.hpp>

#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/smr.hpp>

#include <boost/hana/ext/std/integral_constant.hpp>
#include <boost/hana/integral_constant.hpp>

#include <boost/assert.hpp>
#include <type_traits>
#include <array>

HBRS_MPL_NAMESPACE_BEGIN

template<
	typename Ring,
	std::size_t Rows,
	std::size_t Columns,
	storage_order Order
>
struct ctsam {
	constexpr
	ctsam() : data_{0} {}
	
	constexpr 
	ctsam(std::array<Ring, Rows*Columns> data) : data_{data} {}
	
	constexpr
	ctsam(ctsam const&) = default;
	constexpr
	ctsam(ctsam &&) = default;
	
	constexpr ctsam&
	operator=(ctsam const&) = default;
	constexpr ctsam&
	operator=(ctsam &&) = default;
	
	constexpr matrix_size<hana::size_t<Rows>, hana::size_t<Columns>>
	size() const { return {}; };
	
	constexpr storage_order_<Order>
	order() const { return {}; }
	
	template<typename Index>
	constexpr decltype(auto)
	at(Index && i) {
		return data_[
			detail::translate_index(hana::size_c<Rows>, hana::size_c<Columns>, HBRS_MPL_FWD(i), storage_order_c<Order>)
		];
	}
	
	template<typename Index>
	constexpr decltype(auto)
	at(Index && i) const {
		return data_[
			detail::translate_index(hana::size_c<Rows>, hana::size_c<Columns>, HBRS_MPL_FWD(i), storage_order_c<Order>)
		];
	}
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) & { return smr<ctsam &, std::decay_t<Index>>{*this, HBRS_MPL_FWD(i)}; }
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) const& { return smr<ctsam const&, std::decay_t<Index>>{*this, HBRS_MPL_FWD(i)}; }
	
	template<typename Index>
	constexpr auto
	operator[](Index && i) && { return make_smr(std::move(*this), HBRS_MPL_FWD(i)); }
	
private:
	std::array<Ring, Rows * Columns> data_;
};

HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <
	typename Ring,
	std::size_t Rows,
	std::size_t Columns,
	hbrs::mpl::storage_order Order
>
struct tag_of< hbrs::mpl::ctsam<Ring, Rows, Columns, Order> > {
	using type = hbrs::mpl::ctsam_tag;
};

template <>
struct make_impl<hbrs::mpl::ctsam_tag> {
	template <
		typename Ring,
		typename Rows,
		typename Columns,
		hbrs::mpl::storage_order Order,
		typename std::enable_if_t<
			(
				std::is_same< hana::tag_of_t<Rows>, hana::integral_constant_tag<std::size_t> >::value ||
				std::is_same< hana::tag_of_t<Rows>, hana::ext::std::integral_constant_tag<std::size_t> >::value
			) && (
				std::is_same< hana::tag_of_t<Columns>, hana::integral_constant_tag<std::size_t> >::value ||
				std::is_same< hana::tag_of_t<Columns>, hana::ext::std::integral_constant_tag<std::size_t> >::value
			)
		>* = nullptr
	>
	static constexpr hbrs::mpl::ctsam<Ring, Rows::value, Columns::value, Order>
	apply(
		std::array<Ring, Rows::value * Columns::value> data,
		hbrs::mpl::matrix_size<Rows, Columns>,
		hbrs::mpl::storage_order_<Order>
	) {
		return {data};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_CTSAM_IMPL_HPP
