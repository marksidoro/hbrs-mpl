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

#ifndef HBRS_MPL_DETAIL_INDEX_OF_HPP
#define HBRS_MPL_DETAIL_INDEX_OF_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/not_equal.hpp>
#include <boost/hana/drop_while.hpp>
#include <boost/hana/minus.hpp>
#include <boost/hana/integral_constant.hpp>

#include <boost/hana/tuple.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

/* Src.: https://stackoverflow.com/a/33987589/6490710 */
template <typename Iterable, typename T>
constexpr auto index_of(Iterable const& iterable, T const& element) {
	auto size = decltype(hana::size(iterable)){};
	auto dropped = decltype(hana::size(
		hana::drop_while(iterable, hana::not_equal.to(element))
	)){};
	return size - dropped;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_INDEX_OF_HPP
