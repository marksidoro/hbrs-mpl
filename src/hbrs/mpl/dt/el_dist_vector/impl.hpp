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

#ifndef HBRS_MPL_DT_EL_DIST_VECTOR_IMPL_HPP
#define HBRS_MPL_DT_EL_DIST_VECTOR_IMPL_HPP

#include "fwd.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/matrix_distribution.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <initializer_list>

#define _HBRS_MPL_DEF_EL_DIST_VECTOR(vector_kind)                                                                      \
	HBRS_MPL_NAMESPACE_BEGIN                                                                                           \
	                                                                                                                   \
	template<                                                                                                          \
		typename Ring = double,                                                                                        \
		El::Dist Columnwise = El::MC,                                                                                  \
		El::Dist Rowwise = El::MR,                                                                                     \
		El::DistWrap Wrapping = El::ELEMENT                                                                            \
	>                                                                                                                  \
	struct el_dist_ ## vector_kind ## _vector {                                                                        \
		template<                                                                                                      \
			typename Ring_ = Ring,                                                                                     \
			typename std::enable_if_t<                                                                                 \
				std::is_same_v<std::remove_const_t<Ring>, Ring_>                                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		el_dist_ ## vector_kind ## _vector(El::DistMatrix<Ring_, Columnwise, Rowwise, Wrapping> data);                 \
		el_dist_ ## vector_kind ## _vector(El::Grid const& grid, El::Int sz);                                          \
		                                                                                                               \
		el_dist_ ## vector_kind ## _vector(el_dist_ ## vector_kind ## _vector const&) = default;                       \
		el_dist_ ## vector_kind ## _vector(el_dist_ ## vector_kind ## _vector &&) = default;                           \
		                                                                                                               \
		el_dist_ ## vector_kind ## _vector&                                                                            \
		operator=(el_dist_ ## vector_kind ## _vector const&) = default;                                                \
		el_dist_ ## vector_kind ## _vector&                                                                            \
		operator=(el_dist_ ## vector_kind ## _vector &&) = default;                                                    \
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
	HBRS_MPL_NAMESPACE_END                                                                                             \
	                                                                                                                   \
	BOOST_HANA_NAMESPACE_BEGIN                                                                                         \
                                                                                                                       \
	template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>                             \
	struct tag_of< hbrs::mpl::el_dist_ ## vector_kind ## _vector<Ring, Columnwise, Rowwise, Wrapping> > {              \
		using type = hbrs::mpl::el_dist_ ## vector_kind ## _vector_tag;                                                \
	};                                                                                                                 \
                                                                                                                       \
	template <>                                                                                                        \
	struct make_impl<hbrs::mpl::el_dist_ ## vector_kind ## _vector_tag> {                                              \
		template <typename Ring>                                                                                       \
		static auto                                                                                                    \
		apply(                                                                                                         \
			El::Grid const& grid,                                                                                      \
			std::initializer_list<Ring> data                                                                           \
		) {                                                                                                            \
			hbrs::mpl::el_dist_ ## vector_kind ## _vector<                                                             \
				Ring, El::STAR, El::STAR, El::ELEMENT                                                                  \
			> m{grid, (El::Int)data.size()};                                                                           \
			m.data().Matrix() = hbrs::mpl::make_el_ ## vector_kind ## _vector(data).data();                            \
			return m;                                                                                                  \
		}                                                                                                              \
		                                                                                                               \
		template <typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>                         \
		static hbrs::mpl::el_dist_ ## vector_kind ## _vector<Ring, Columnwise, Rowwise, Wrapping>                      \
		apply(El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> data) {                                              \
			return { std::move(data) };                                                                                \
		}                                                                                                              \
		                                                                                                               \
		template <typename Ring>                                                                                       \
		static hbrs::mpl::el_dist_ ## vector_kind ## _vector<Ring, El::STAR, El::STAR, El::ELEMENT>                    \
		apply(                                                                                                         \
			El::Grid const& grid,                                                                                      \
			hbrs::mpl::el_ ## vector_kind ## _vector<Ring> const& local                                                \
		) {                                                                                                            \
			El::DistMatrix<Ring, El::STAR, El::STAR, El::ELEMENT> dmat{grid};                                          \
			dmat.Resize(local.data().Height(), local.data().Width());                                                  \
			dmat.Matrix() = local.data();                                                                              \
			return { std::move(dmat) };                                                                                \
		}                                                                                                              \
		                                                                                                               \
		template <                                                                                                     \
			typename Ring,                                                                                             \
			El::Dist FromColumnwise, El::Dist FromRowwise, El::DistWrap FromWrapping,                                  \
			El::Dist ToColumnwise, El::Dist ToRowwise, El::DistWrap ToWrapping                                         \
		>                                                                                                              \
		static hbrs::mpl::el_dist_ ## vector_kind ## _vector<Ring, ToColumnwise, ToRowwise, ToWrapping>                \
		apply(                                                                                                         \
			hbrs::mpl::el_dist_ ## vector_kind ## _vector<Ring, FromColumnwise, FromRowwise, FromWrapping> && v,       \
			hbrs::mpl::matrix_distribution<                                                                            \
				integral_constant<El::Dist, ToColumnwise>,                                                             \
				integral_constant<El::Dist, ToRowwise>,                                                                \
				integral_constant<El::DistWrap, ToWrapping>                                                            \
			>                                                                                                          \
		) {                                                                                                            \
			return { HBRS_MPL_FWD(v).data() };                                                                         \
		}                                                                                                              \
		                                                                                                               \
		template <                                                                                                     \
			typename Ring,                                                                                             \
			El::Dist FromColumnwise, El::Dist FromRowwise, El::DistWrap FromWrapping,                                  \
			El::Dist ToColumnwise, El::Dist ToRowwise, El::DistWrap ToWrapping                                         \
		>                                                                                                              \
		static hbrs::mpl::el_dist_ ## vector_kind ## _vector<Ring, ToColumnwise, ToRowwise, ToWrapping>                \
		apply(                                                                                                         \
			hbrs::mpl::el_dist_ ## vector_kind ## _vector<Ring, FromColumnwise, FromRowwise, FromWrapping> const& v,   \
			hbrs::mpl::matrix_distribution<                                                                            \
				integral_constant<El::Dist, ToColumnwise>,                                                             \
				integral_constant<El::Dist, ToRowwise>,                                                                \
				integral_constant<El::DistWrap, ToWrapping>                                                            \
			>                                                                                                          \
		) {                                                                                                            \
			return { v.data() };                                                                                       \
		}                                                                                                              \
	};                                                                                                                 \
                                                                                                                       \
	BOOST_HANA_NAMESPACE_END

_HBRS_MPL_DEF_EL_DIST_VECTOR(column)
_HBRS_MPL_DEF_EL_DIST_VECTOR(row)

#undef _HBRS_MPL_DEF_EL_DIST_VECTOR

HBRS_MPL_NAMESPACE_BEGIN

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
template<
	typename Ring_,
	typename std::enable_if_t<
		std::is_same_v<std::remove_const_t<Ring>, Ring_>
	>*
>
el_dist_column_vector<Ring, Columnwise, Rowwise, Wrapping>::
el_dist_column_vector(El::DistMatrix<Ring_, Columnwise, Rowwise, Wrapping> data) : data_{data} {
	BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	BOOST_ASSERT(data_.Width() == 1);
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
el_dist_column_vector<Ring, Columnwise, Rowwise, Wrapping>::
el_dist_column_vector(El::Grid const& grid, El::Int sz) : data_{grid} {
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
el_dist_row_vector<Ring, Columnwise, Rowwise, Wrapping>::
el_dist_row_vector(El::DistMatrix<Ring_, Columnwise, Rowwise, Wrapping> data) : data_{data} {
	BOOST_ASSERT(!std::is_const_v<Ring> ? !data_.Locked() : true);
	BOOST_ASSERT(data_.Height() == 1);
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
el_dist_row_vector<Ring, Columnwise, Rowwise, Wrapping>::
el_dist_row_vector(El::Grid const& grid, El::Int sz) : data_{grid} {
	data_.Resize(1, sz);
	El::Zero(data_);
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
decltype(auto)
el_dist_column_vector<Ring, Columnwise, Rowwise, Wrapping>::length() const {
	return data_.Height();
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
decltype(auto)
el_dist_row_vector<Ring, Columnwise, Rowwise, Wrapping>::length() const {
	return data_.Width();
}

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_DT_EL_DIST_VECTOR_IMPL_HPP
