/* Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_DETAIL_VECTOR_FWD_HPP
#define HBRS_MPL_DETAIL_VECTOR_FWD_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/fn/size/fwd.hpp>
#include <hbrs/mpl/fn/at/fwd.hpp>

#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <boost/hana/type.hpp>
#include <boost/assert.hpp>
#include <type_traits>

#define HBRS_MPL_DEC_SV(cname)                                                                                         \
	HBRS_MPL_NAMESPACE_BEGIN                                                                                           \
                                                                                                                       \
	template<typename Sequence>                                                                                        \
	struct cname {                                                                                                     \
		template<                                                                                                      \
			typename Sequence_ = Sequence,                                                                             \
			typename std::enable_if_t<                                                                                 \
				detail::is_braces_constructible<Sequence, Sequence_&&>::value &&                                       \
				!std::is_base_of<cname, std::remove_reference_t<Sequence_> >::value                                    \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		constexpr                                                                                                      \
		cname(Sequence_ && seq) : seq_{HBRS_MPL_FWD(seq)} {}                                                           \
		                                                                                                               \
		constexpr                                                                                                      \
		cname(cname const&) = default;                                                                                 \
		constexpr                                                                                                      \
		cname(cname &&) = default;                                                                                     \
		                                                                                                               \
		constexpr cname&                                                                                               \
		operator=(cname const&) = default;                                                                             \
		constexpr cname&                                                                                               \
		operator=(cname &&) = default;                                                                                 \
		                                                                                                               \
		constexpr decltype(auto)                                                                                       \
		size() const;                                                                                                  \
		                                                                                                               \
		template<typename Index>                                                                                       \
		constexpr decltype(auto)                                                                                       \
		at(Index && i);                                                                                                \
		                                                                                                               \
		template<typename Index>                                                                                       \
		constexpr decltype(auto)                                                                                       \
		at(Index && i) const;                                                                                          \
		                                                                                                               \
		template<typename Index>                                                                                       \
		constexpr decltype(auto)                                                                                       \
		operator[](Index && i);                                                                                        \
		                                                                                                               \
		template<typename Index>                                                                                       \
		constexpr decltype(auto)                                                                                       \
		operator[](Index && i) const;                                                                                  \
		                                                                                                               \
	private:                                                                                                           \
		Sequence seq_;                                                                                                 \
	};                                                                                                                 \
                                                                                                                       \
	HBRS_MPL_NAMESPACE_END                                                                                             \
                                                                                                                       \
	namespace boost { namespace hana {                                                                                 \
                                                                                                                       \
	template <typename Sequence>                                                                                       \
	struct tag_of< hbrs::mpl::cname<Sequence> > {                                                                      \
		using type = hbrs::mpl::cname ## _tag;                                                                         \
	};                                                                                                                 \
                                                                                                                       \
	template <>                                                                                                        \
	struct make_impl<hbrs::mpl::cname ## _tag> {                                                                       \
		template <typename Sequence>                                                                                   \
		static constexpr hbrs::mpl::cname<std::decay_t<Sequence>>                                                      \
		apply(Sequence && seq) {                                                                                       \
			return {HBRS_MPL_FWD(seq)};                                                                                \
		}                                                                                                              \
	};                                                                                                                 \
                                                                                                                       \
	/* namespace hana */ } /* namespace boost */ }

#define HBRS_MPL_DEF_SV(cname)                                                                                         \
	HBRS_MPL_NAMESPACE_BEGIN                                                                                           \
	                                                                                                                   \
	template<typename Sequence>                                                                                        \
	constexpr decltype(auto)                                                                                           \
	cname<Sequence>::size() const { return (*hbrs::mpl::size)(seq_); }                                                 \
	                                                                                                                   \
	template<typename Sequence>                                                                                        \
	template<typename Index>                                                                                           \
	constexpr decltype(auto)                                                                                           \
	cname<Sequence>::at(Index && i) { return (*hbrs::mpl::at)(seq_, HBRS_MPL_FWD(i)); }                                \
	                                                                                                                   \
	template<typename Sequence>                                                                                        \
	template<typename Index>                                                                                           \
	constexpr decltype(auto)                                                                                           \
	cname<Sequence>::at(Index && i) const { return (*hbrs::mpl::at)(seq_, HBRS_MPL_FWD(i)); }                          \
	                                                                                                                   \
	template<typename Sequence>                                                                                        \
	template<typename Index>                                                                                           \
	constexpr decltype(auto)                                                                                           \
	cname<Sequence>::operator[](Index && i) { return at(HBRS_MPL_FWD(i)); }                                            \
	                                                                                                                   \
	template<typename Sequence>                                                                                        \
	template<typename Index>                                                                                           \
	constexpr decltype(auto)                                                                                           \
	cname<Sequence>::operator[](Index && i) const { return at(HBRS_MPL_FWD(i)); }                                      \
	                                                                                                                   \
	HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_VECTOR_FWD_HPP
