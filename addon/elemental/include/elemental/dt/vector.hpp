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

#ifndef ELEMENTAL_DT_VECTOR_HPP
#define ELEMENTAL_DT_VECTOR_HPP

#include <elemental/fwd/dt/vector.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>

#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/size.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/hana/type.hpp>
#include <boost/assert.hpp>
#include <El.hpp>

#define _ELEMENTAL_DEF_VECTOR(vector_kind)                                                                             \
	ELEMENTAL_NAMESPACE_BEGIN                                                                                          \
	namespace mpl = hbrs::mpl;                                                                                         \
                                                                                                                       \
	template<typename Ring>                                                                                            \
	struct vector_kind ## _vector {                                                                                    \
		vector_kind ## _vector(El::Matrix<Ring> data);                                                                 \
		vector_kind ## _vector(El::Int sz);                                                                            \
		                                                                                                               \
		vector_kind ## _vector(Ring const* data, El::Int sz)                                                           \
		: vector_kind ## _vector{sz} {                                                                                 \
			for(El::Int i = 0; i < sz; ++i) {                                                                          \
				at(i) = data[i];                                                                                       \
			}                                                                                                          \
		}                                                                                                              \
		                                                                                                               \
		vector_kind ## _vector(vector_kind ## _vector const&) = default;                                               \
		vector_kind ## _vector(vector_kind ## _vector &&) = default;                                                   \
		                                                                                                               \
		vector_kind ## _vector&                                                                                        \
		operator=(vector_kind ## _vector const&) = default;                                                            \
		vector_kind ## _vector&                                                                                        \
		operator=(vector_kind ## _vector &&) = default;                                                                \
		                                                                                                               \
		decltype(auto)                                                                                                 \
		length() const;                                                                                                \
		                                                                                                               \
		decltype(auto)                                                                                                 \
		at(El::Int i);                                                                                                 \
                                                                                                                       \
		decltype(auto)                                                                                                 \
		at(El::Int i) const;                                                                                           \
		                                                                                                               \
		decltype(auto)                                                                                                 \
		operator[](El::Int i) {                                                                                        \
			return at(i);                                                                                              \
		}                                                                                                              \
		                                                                                                               \
		decltype(auto)                                                                                                 \
		operator[](El::Int i) const {                                                                                  \
			return at(i);                                                                                              \
		}                                                                                                              \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		data() & { return (data_); };                                                                                  \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		data() const& { return (data_); };                                                                             \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		data() && { return HBRS_MPL_FWD(data_); };                                                                     \
		                                                                                                               \
	private:                                                                                                           \
		El::Matrix<Ring> data_;                                                                                        \
	};                                                                                                                 \
	                                                                                                                   \
	ELEMENTAL_NAMESPACE_END                                                                                            \
                                                                                                                       \
	namespace boost { namespace hana {                                                                                 \
                                                                                                                       \
	template <typename Ring>                                                                                           \
	struct tag_of< elemental::vector_kind ## _vector<Ring> > {                                                         \
		using type = elemental::vector_kind ## _vector_tag;                                                            \
	};                                                                                                                 \
                                                                                                                       \
	template <>                                                                                                        \
	struct make_impl<elemental::vector_kind ## _vector_tag> {                                                          \
		template <typename Ring>                                                                                       \
		static elemental::vector_kind ## _vector<Ring>                                                                 \
		apply(basic_type<Ring>, El::Int sz) {                                                                          \
			return {sz};                                                                                               \
		}                                                                                                              \
	                                                                                                                   \
		template <typename Ring>                                                                                       \
		static elemental::vector_kind ## _vector<std::decay_t<Ring>>                                                   \
		apply(El::Matrix<Ring> data) {                                                                                 \
			return {data};                                                                                             \
		}                                                                                                              \
		                                                                                                               \
		template <typename Ring>                                                                                       \
		static elemental::vector_kind ## _vector<std::decay_t<Ring>>                                                   \
		apply(Ring const* data, El::Int sz) {                                                                          \
			return {data, sz};                                                                                         \
		}                                                                                                              \
		                                                                                                               \
		template <typename Ring>                                                                                       \
		static elemental::vector_kind ## _vector<std::decay_t<Ring>>                                                   \
		apply(std::initializer_list<Ring> data) {                                                                      \
			return { data.begin(), boost::numeric_cast<El::Int>(data.size()) };                                        \
		}                                                                                                              \
	};                                                                                                                 \
                                                                                                                       \
	/* namespace hana */ } /* namespace boost */ }

_ELEMENTAL_DEF_VECTOR(column)
_ELEMENTAL_DEF_VECTOR(row)

#undef _ELEMENTAL_DEF_VECTOR

ELEMENTAL_NAMESPACE_BEGIN

template<typename Ring>
column_vector<Ring>::column_vector(El::Matrix<Ring> data) : data_{data} {
	BOOST_ASSERT(data_.Width() == 1);
}

template<typename Ring>
row_vector<Ring>::row_vector(El::Matrix<Ring> data) : data_{data} {
	BOOST_ASSERT(data_.Height() == 1);
}

template<typename Ring>
column_vector<Ring>::column_vector(El::Int sz) : data_{sz,1} {
	El::Zero(data_);
}

template<typename Ring>
row_vector<Ring>::row_vector(El::Int sz) : data_{1,sz} {
	El::Zero(data_);
}

template<typename Ring>
decltype(auto)
column_vector<Ring>::length() const {
	return data_.Height();
}

template<typename Ring>
decltype(auto)
row_vector<Ring>::length() const {
	return data_.Width();
}

template<typename Ring>
decltype(auto)
column_vector<Ring>::at(El::Int i) {
	using hbrs::mpl::at;
	return (*at)(data_, mpl::make_matrix_index(i, 0));
}

template<typename Ring>
decltype(auto)
column_vector<Ring>::at(El::Int i) const {
	using hbrs::mpl::at;
	return (*at)(data_, mpl::make_matrix_index(i, 0));
}

template<typename Ring>
decltype(auto)
row_vector<Ring>::at(El::Int i) {
	using hbrs::mpl::at;
	return (*at)(data_, mpl::make_matrix_index(0, i));
}

template<typename Ring>
decltype(auto)
row_vector<Ring>::at(El::Int i) const {
	using hbrs::mpl::at;
	return (*at)(data_, mpl::make_matrix_index(0, i));
}

ELEMENTAL_NAMESPACE_END

#endif // !ELEMENTAL_DT_VECTOR_HPP
