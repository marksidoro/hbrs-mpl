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

#pragma once

#ifndef HBRS_MPL_DETAIL_TRANSLATE_INDEX_HPP
#define HBRS_MPL_DETAIL_TRANSLATE_INDEX_HPP

#include <hbrs/mpl/fwd/detail/translate_index.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/plus.hpp>

#include <hbrs/mpl/dt/storage_order.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

struct row_major_index_translator {
	template<typename Rows, typename Columns, typename RowIndex, typename ColumnIndex>
	constexpr decltype(auto)
	operator()(Rows && rows, Columns && cols, RowIndex && i, ColumnIndex && j) {
		// i * cols + j
		return (*plus)(multiply(HBRS_MPL_FWD(i), HBRS_MPL_FWD(cols)), HBRS_MPL_FWD(j));
	}
};

struct column_major_index_translator {
	template<typename Rows, typename Columns, typename RowIndex, typename ColumnIndex>
	constexpr decltype(auto)
	operator()(Rows && rows, Columns && cols, RowIndex && i, ColumnIndex && j) {
		// j * rows + i
		return (*plus)(multiply(HBRS_MPL_FWD(j), HBRS_MPL_FWD(rows)), HBRS_MPL_FWD(i));
	}
};

template<typename Rows, typename Columns, typename RowIndex, typename ColumnIndex>
constexpr decltype(auto)
translate_index(Rows && rows, Columns && cols, RowIndex && i, ColumnIndex && j, row_major_) {
	return row_major_index_translator{}(
		HBRS_MPL_FWD(rows),
		HBRS_MPL_FWD(cols),
		HBRS_MPL_FWD(i),
		HBRS_MPL_FWD(j)
	);
}

template<typename Rows, typename Columns, typename RowIndex, typename ColumnIndex>
constexpr decltype(auto)
translate_index(Rows && rows, Columns && cols, RowIndex && i, ColumnIndex && j, column_major_) {
	return column_major_index_translator{}(
		HBRS_MPL_FWD(rows),
		HBRS_MPL_FWD(cols),
		HBRS_MPL_FWD(i),
		HBRS_MPL_FWD(j)
	);
}

template<
	typename Size,
	typename Index,
	typename StorageOrder,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Size>, matrix_size_tag >::value &&
		std::is_same< hana::tag_of_t<Index>, matrix_index_tag >::value
	>* /*= nullptr*/
>
constexpr decltype(auto)
translate_index(Size && sz, Index && ix, StorageOrder && so) {
	return translate_index(
		HBRS_MPL_FWD(sz).m(),
		HBRS_MPL_FWD(sz).n(),
		HBRS_MPL_FWD(ix).m(),
		HBRS_MPL_FWD(ix).n(),
		HBRS_MPL_FWD(so)
	);
}

template<
	typename Size,
	typename RowIndex,
	typename ColumnIndex,
	typename StorageOrder,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Size>, matrix_size_tag >::value
	>* /*= nullptr*/
>
constexpr decltype(auto)
translate_index(Size && sz, RowIndex && i, ColumnIndex && j, StorageOrder && so) {
	return translate_index(
		HBRS_MPL_FWD(sz).m(),
		HBRS_MPL_FWD(sz).n(),
		HBRS_MPL_FWD(i),
		HBRS_MPL_FWD(j),
		HBRS_MPL_FWD(so)
	);
}

template<
	typename Columns,
	typename Rows,
	typename Index,
	typename StorageOrder,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Index>, matrix_index_tag >::value
	>* /*= nullptr*/
>
constexpr decltype(auto)
translate_index(Rows && rows, Columns && cols, Index && ix, StorageOrder && so) {
	return translate_index(
		HBRS_MPL_FWD(rows),
		HBRS_MPL_FWD(cols),
		HBRS_MPL_FWD(ix).m(),
		HBRS_MPL_FWD(ix).n(),
		HBRS_MPL_FWD(so)
	);
}

template<
	typename Size,
	typename Index,
	typename StorageOrder,
	typename std::enable_if_t< 
		!std::is_same< hana::tag_of_t<Size>, matrix_size_tag >::value ||
		!std::is_same< hana::tag_of_t<Index>, matrix_index_tag >::value
	>* /*= nullptr*/
>
constexpr decltype(auto)
translate_index(Size && sz, Index && ix, StorageOrder && so) {
	return translate_index(
		HBRS_MPL_FWD(sz).m(),
		HBRS_MPL_FWD(sz).n(),
		HBRS_MPL_FWD(ix).m(),
		HBRS_MPL_FWD(ix).n(),
		HBRS_MPL_FWD(so)
	);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END


#endif // !HBRS_MPL_DETAIL_TRANSLATE_INDEX_HPP
