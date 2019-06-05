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

#ifndef ELEMENTAL_DT_DIST_VECTOR_HPP
#define ELEMENTAL_DT_DIST_VECTOR_HPP

#include <elemental/fwd/dt/dist_vector.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <elemental/dt/matrix.hpp>
#include <initializer_list>

//TODO: replace this hack!

#define _ELEMENTAL_DEF_DIST_VECTOR(vector_kind)                                                                        \
	ELEMENTAL_NAMESPACE_BEGIN                                                                                          \
	                                                                                                                   \
	template<typename Matrix>                                                                                          \
	struct dist_ ## vector_kind ## _vector {                                                                          \
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
		decltype(auto)                                                                                                 \
		length() const;                                                                                                \
		                                                                                                               \
		Matrix data_;                                                                                                  \
	};                                                                                                                 \
	                                                                                                                   \
	ELEMENTAL_NAMESPACE_END                                                                                            \
	                                                                                                                   \
	namespace boost { namespace hana {                                                                                 \
                                                                                                                       \
	template <typename Matrix>                                                                                         \
	struct tag_of< elemental::dist_ ## vector_kind ## _vector<Matrix> > {                                              \
		using type = elemental::dist_ ## vector_kind ## _vector_tag;                                                   \
	};                                                                                                                 \
                                                                                                                       \
	/* namespace hana */ } /* namespace boost */ }

_ELEMENTAL_DEF_DIST_VECTOR(column)
_ELEMENTAL_DEF_DIST_VECTOR(row)

#undef _ELEMENTAL_DEF_DIST_VECTOR

ELEMENTAL_NAMESPACE_BEGIN

template<typename Matrix>
decltype(auto)
dist_column_vector<Matrix>::length() const {
	return data_.Height();
}

template<typename Matrix>
decltype(auto)
dist_row_vector<Matrix>::length() const {
	return data_.Width();
}

ELEMENTAL_NAMESPACE_END

namespace boost { namespace hana {

template <>
struct make_impl<elemental::dist_column_vector_tag> {
	//TODO: replace this hack!
	template <typename Ring>
	static auto
	apply(
		El::Grid const& grid,
		std::initializer_list<Ring> data
	) {
		typedef El::DistMatrix<Ring, El::STAR, El::STAR, El::ELEMENT> Matrix;
		
		Matrix dmat{grid};
		dmat.Resize((El::Int)data.size(), 1);
		dmat.Matrix() = elemental::make_matrix(
			data,
			hbrs::mpl::matrix_size<El::Int, El::Int>{(El::Int)data.size(), 1},
			hbrs::mpl::row_major_c
		).data();
		
		return elemental::dist_column_vector<Matrix>{dmat};
	}
};

template <>
struct make_impl<elemental::dist_row_vector_tag> {
	//TODO: replace this hack!
	template <typename Ring>
	static auto
	apply(
		El::Grid const& grid,
		std::initializer_list<Ring> data
	) {
		typedef El::DistMatrix<Ring, El::STAR, El::STAR, El::ELEMENT> Matrix;
		
		Matrix dmat{grid};
		dmat.Resize(1, (El::Int)data.size());
		dmat.Matrix() = elemental::make_matrix(
			data,
			hbrs::mpl::matrix_size<El::Int, El::Int>{1, (El::Int)data.size()},
			hbrs::mpl::row_major_c
		).data();
		
		return elemental::dist_row_vector<Matrix>{dmat};
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !ELEMENTAL_DT_DIST_VECTOR_HPP
