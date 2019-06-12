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

#ifndef HBRS_MPL_DETAIL_COPY_MATRIX_HPP
#define HBRS_MPL_DETAIL_COPY_MATRIX_HPP

#include <hbrs/mpl/fwd/detail/copy_matrix.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>

#include <hbrs/mpl/detail/translate_index.hpp>

#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>

#include <hbrs/mpl/dt/exception.hpp>
#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/assert.hpp>

#include <iterator>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {


template <
	typename From,
	typename std::enable_if_t<
		std::is_integral<From>::value
	>* = nullptr
>
auto
to_std_size_t(From const& from) {
	return boost::numeric_cast<std::size_t>(from);
}

template <
	typename From,
	typename std::enable_if_t<
		hana::IntegralConstant<From>::value &&
		std::is_convertible<typename std::remove_reference_t<From>::value_type, std::size_t>::value
	>* = nullptr
>
auto
to_std_size_t(From const& from) {
	return to_std_size_t(From::value);
}

template <
	typename From, 
	typename To,
	typename std::enable_if_t<
		!std::is_rvalue_reference<To&&>::value
	>* = nullptr
>
decltype(auto)
copy_matrix_impl(
	From const& from,
	To && to
) {
	using boost::numeric_cast;
	
	auto const from_sz = (*size)(from);
	auto const from_m = to_std_size_t((*m)(from_sz));
	auto const from_n = to_std_size_t((*n)(from_sz));
	
	auto const to_sz = (*size)(to);
	auto const to_m = to_std_size_t((*m)(to_sz));
	auto const to_n = to_std_size_t((*n)(to_sz));
	
	auto from_sz_u = make_matrix_size(from_m, from_n);
	auto to_sz_u =   make_matrix_size(to_m, to_n);
	
	if (from_sz_u != to_sz_u) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrices_exception{}
			<< errinfo_matrix_sizes{{from_sz_u, to_sz_u}}
		));
	}
	
	//TODO: replace with transform/? which is storage order agnostic
	for(std::size_t i = 0; i < from_m; ++i) {
		for(std::size_t j = 0; j < from_n; ++j) {
			(*at)(to, mpl::make_matrix_index(i,j)) = (*at)(from, mpl::make_matrix_index(i,j));
		}
	}
	
	return HBRS_MPL_FWD(to);
}

template <typename From, typename To>
constexpr decltype(auto)
copy_matrix(From const& from, To && to) {
	return copy_matrix_impl(from, to);
}

template <typename From, typename To>
constexpr decltype(auto)
copy_matrix(From const& from, To & to) {
	return copy_matrix_impl(from, to);
}

template <
	typename From,
	storage_order Order,
	typename To,
	typename std::enable_if_t<
		!std::is_rvalue_reference<To&&>::value
	>* = nullptr
>
constexpr decltype(auto)
copy_matrix_impl(
	std::initializer_list<From> from,
	storage_order_<Order>,
	To && to
) {
	using boost::numeric_cast;
	
	auto const to_sz = (*size)(to);
	auto const to_m = (*m)(to_sz);
	auto const to_n = (*n)(to_sz);
	
	typedef std::decay_t<decltype(to_m)> ToM;
	typedef std::decay_t<decltype(to_n)> ToN;
	
	static_assert(std::is_convertible<std::size_t, ToM>::value && std::is_convertible<std::size_t, ToN>::value, "");
	
	auto const to_m_u = to_std_size_t(to_m);
	auto const to_n_u = to_std_size_t(to_n);
	
	if (to_m_u * to_n_u != from.size()) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrix_sequence_exception{}
			<< errinfo_matrix_size{{to_m_u, to_n_u}}
			<< errinfo_sequence_size{from.size()}
		));
	}
	
	//TODO: replace with fold which is storage order agnostic
	for(std::size_t i = 0; i < to_m_u; ++i) {
		for(std::size_t j = 0; j < to_n_u; ++j) {
			auto ix = translate_index(to_m_u, to_n_u, i, j, storage_order_c<Order>);
			BOOST_ASSERT(ix >= 0 && ix < to_m_u * to_n_u);
			(*at)(to, make_matrix_index((ToM)i,(ToN)j)) = *std::next(from.begin(), ix);
		};
	};
	
	return to;
}

template <typename From, storage_order Order, typename To>
constexpr decltype(auto)
copy_matrix(std::initializer_list<From> from, storage_order_<Order>, To && to) {
	return copy_matrix_impl(from, storage_order_c<Order>, to);
}

template <typename From, storage_order Order, typename To>
constexpr decltype(auto)
copy_matrix(std::initializer_list<From> from, storage_order_<Order>, To & to) {
	return copy_matrix_impl(from, storage_order_c<Order>, to);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END


#endif // !HBRS_MPL_DETAIL_COPY_MATRIX_HPP
