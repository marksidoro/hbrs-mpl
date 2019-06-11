/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef ELEMENTAL_DT_DIST_VECTOR_HPP
#define ELEMENTAL_DT_DIST_VECTOR_HPP

#include <elemental/fwd/dt/dist_vector.hpp>
#include <elemental/dt/vector.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <initializer_list>

#define _ELEMENTAL_DEF_DIST_VECTOR(vector_kind)                                                                        \
	ELEMENTAL_NAMESPACE_BEGIN                                                                                          \
	                                                                                                                   \
	template<                                                                                                          \
		typename Ring = double,                                                                                        \
		El::Dist Columnwise = El::MC,                                                                                  \
		El::Dist Rowwise = El::MR,                                                                                     \
		El::DistWrap Wrapping = El::ELEMENT                                                                            \
	>                                                                                                                  \
	struct dist_ ## vector_kind ## _vector {                                                                           \
		template<                                                                                                      \
			typename Ring_ = Ring,                                                                                     \
			typename std::enable_if_t<                                                                                 \
				std::is_same_v<std::remove_const_t<Ring>, Ring_>                                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		dist_ ## vector_kind ## _vector(El::DistMatrix<Ring_, Columnwise, Rowwise, Wrapping> data);                    \
		dist_ ## vector_kind ## _vector(El::Grid const& grid, El::Int sz);                                             \
		                                                                                                               \
		dist_ ## vector_kind ## _vector(dist_ ## vector_kind ## _vector const&) = default;                             \
		dist_ ## vector_kind ## _vector(dist_ ## vector_kind ## _vector &&) = default;                                 \
		                                                                                                               \
		dist_ ## vector_kind ## _vector&                                                                               \
		operator=(dist_ ## vector_kind ## _vector const&) = default;                                                   \
		dist_ ## vector_kind ## _vector&                                                                               \
		operator=(dist_ ## vector_kind ## _vector &&) = default;                                                       \
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
	private:                                                                                                           \
		El::DistMatrix<                                                                                                \
			std::remove_const_t<Ring>,                                                                                 \
			Columnwise, Rowwise, Wrapping                                                                              \
		> data_;                                                                                                       \
	};                                                                                                                 \
	                                                                                                                   \
	ELEMENTAL_NAMESPACE_END                                                                                            \
	                                                                                                                   \
	BOOST_HANA_NAMESPACE_BEGIN                                                                                         \
                                                                                                                       \
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>                             \
	struct tag_of< elemental::dist_ ## vector_kind ## _vector<Ring, Columnwise, Rowwise, Wrapping> > {                 \
		using type = elemental::dist_ ## vector_kind ## _vector_tag;                                                   \
	};                                                                                                                 \
                                                                                                                       \
	template <>                                                                                                        \
	struct make_impl<elemental::dist_ ## vector_kind ## _vector_tag> {                                                 \
		template <typename Ring>                                                                                       \
		static auto                                                                                                    \
		apply(                                                                                                         \
			El::Grid const& grid,                                                                                      \
			std::initializer_list<Ring> data                                                                           \
		) {                                                                                                            \
			elemental::dist_ ## vector_kind ## _vector<                                                                \
				Ring, El::STAR, El::STAR, El::ELEMENT                                                                  \
			> m{grid, (El::Int)data.size()};                                                                           \
			m.data().Matrix() = elemental::make_ ## vector_kind ## _vector(data).data();                               \
			return m;                                                                                                  \
		}                                                                                                              \
		                                                                                                               \
		template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>                         \
		static elemental::dist_ ## vector_kind ## _vector<Ring, Columnwise, Rowwise, Wrapping>                         \
		apply(El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> data) {                                              \
			return { data };                                                                                           \
		}                                                                                                              \
	};                                                                                                                 \
                                                                                                                       \
	BOOST_HANA_NAMESPACE_END

_ELEMENTAL_DEF_DIST_VECTOR(column)
_ELEMENTAL_DEF_DIST_VECTOR(row)

#undef _ELEMENTAL_DEF_DIST_VECTOR

ELEMENTAL_NAMESPACE_BEGIN

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
template<
	typename Ring_,
	typename std::enable_if_t<
		std::is_same_v<std::remove_const_t<Ring>, Ring_>
	>*
>
dist_column_vector<Ring, Columnwise, Rowwise, Wrapping>::
dist_column_vector(El::DistMatrix<Ring_, Columnwise, Rowwise, Wrapping> data) : data_{data} {
	BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	BOOST_ASSERT(data_.Width() == 1);
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
dist_column_vector<Ring, Columnwise, Rowwise, Wrapping>::
dist_column_vector(El::Grid const& grid, El::Int sz) : data_{grid} {
	data_.Resize(sz, 1);
	El::Zero(data_);
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
template<
	typename Ring_,
	typename std::enable_if_t<
		std::is_same_v<std::remove_const_t<Ring>, Ring_>
	>*
>
dist_row_vector<Ring, Columnwise, Rowwise, Wrapping>::
dist_row_vector(El::DistMatrix<Ring_, Columnwise, Rowwise, Wrapping> data) : data_{data} {
	BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	BOOST_ASSERT(data_.Height() == 1);
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
dist_row_vector<Ring, Columnwise, Rowwise, Wrapping>::
dist_row_vector(El::Grid const& grid, El::Int sz) : data_{grid} {
	data_.Resize(1, sz);
	El::Zero(data_);
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
decltype(auto)
dist_column_vector<Ring, Columnwise, Rowwise, Wrapping>::length() const {
	return data_.Height();
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
decltype(auto)
dist_row_vector<Ring, Columnwise, Rowwise, Wrapping>::length() const {
	return data_.Width();
}

ELEMENTAL_NAMESPACE_END

#endif // !ELEMENTAL_DT_DIST_VECTOR_HPP
