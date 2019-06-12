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

#ifndef MATLAB_DT_VECTOR_HPP
#define MATLAB_DT_VECTOR_HPP

#include <matlab/fwd/dt/vector.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <algorithm>

#define _MATLAB_DEF_VEC1(vector_kind, base_type)                                                                     \
	MATLAB_NAMESPACE_BEGIN                                                                                             \
	namespace mpl = hbrs::mpl;                                                                                         \
                                                                                                                       \
	template<>                                                                                                         \
	struct vector_kind ## _vector<base_type> {                                                                         \
		                                                                                                               \
		                                                                                                               \
		vector_kind ## _vector()                                                                                       \
		: ptr_{nullptr, nullptr} {                                                                                     \
			emxArray_ ## base_type * emxArray{nullptr};                                                                \
			emxInitArray_ ## base_type(&emxArray, 1);                                                                  \
			ptr_ = {emxArray, emxDestroyArray_ ## base_type};                                                          \
		}                                                                                                              \
		                                                                                                               \
		vector_kind ## _vector(int size)                                                                               \
		: ptr_{emxCreateND_ ## base_type(1, &size), emxDestroyArray_ ## base_type} {}                                  \
		                                                                                                               \
		vector_kind ## _vector(vector_kind ## _vector const& rhs) : vector_kind ## _vector() {                         \
			if (rhs.ptr_ == nullptr) { return; }                                                                       \
			                                                                                                           \
			auto && v = (*rhs.ptr_);                                                                                   \
			auto && sz = rhs.ptr_->size[0];                                                                            \
			                                                                                                           \
			if (v.data != nullptr && sz > 0) {                                                                         \
				ptr_ = {                                                                                               \
					emxCreateND_ ## base_type(1, &sz),                                                                 \
					emxDestroyArray_ ## base_type                                                                      \
				};                                                                                                     \
                                                                                                                       \
				std::copy(v.data, v.data + sz, (*ptr_).data);                                                          \
			}                                                                                                          \
		}                                                                                                              \
		                                                                                                               \
		vector_kind ## _vector(vector_kind ## _vector && rhs) : vector_kind ## _vector() {                             \
			swap(*this, rhs);                                                                                          \
		}                                                                                                              \
		                                                                                                               \
		vector_kind ## _vector&                                                                                        \
		operator=(vector_kind ## _vector rhs) {                                                                        \
			swap(*this, rhs);                                                                                          \
			return *this;                                                                                              \
		}                                                                                                              \
		                                                                                                               \
		friend void swap(vector_kind ## _vector& lhs, vector_kind ## _vector& rhs) noexcept {                          \
			using std::swap;                                                                                           \
			swap(lhs.ptr_, rhs.ptr_);                                                                                  \
		}                                                                                                              \
		                                                                                                               \
		int                                                                                                            \
		length() const {                                                                                               \
			return ptr_->size[0];                                                                                      \
		}                                                                                                              \
		                                                                                                               \
		base_type &                                                                                                    \
		at(int n) {                                                                                                    \
			using hbrs::mpl::at;                                                                                       \
			return (*at)(*ptr_, n);                                                                                    \
		}                                                                                                              \
                                                                                                                       \
		base_type const&                                                                                               \
		at(int n) const {                                                                                              \
			using hbrs::mpl::at;                                                                                       \
			return (*at)(*ptr_, n);                                                                                    \
		}                                                                                                              \
		                                                                                                               \
		base_type &                                                                                                    \
		operator[](int n) {                                                                                            \
			using hbrs::mpl::at;                                                                                       \
			return (*at)(*ptr_, n);                                                                                    \
		}                                                                                                              \
		                                                                                                               \
		base_type const&                                                                                               \
		operator[](int n) const {                                                                                      \
			using hbrs::mpl::at;                                                                                       \
			return (*at)(*ptr_, n);                                                                                    \
		}                                                                                                              \
		                                                                                                               \
		emxArray_ ## base_type &                                                                                       \
		data() {                                                                                                       \
			return *ptr_;                                                                                              \
		}                                                                                                              \
		                                                                                                               \
		emxArray_ ## base_type const&                                                                                  \
		data() const {                                                                                                 \
			return *ptr_;                                                                                              \
		}                                                                                                              \
		                                                                                                               \
	private:                                                                                                           \
		std::unique_ptr<                                                                                               \
			emxArray_ ## base_type,                                                                                    \
			void(*)(emxArray_ ## base_type*)                                                                           \
		> ptr_;                                                                                                        \
	};                                                                                                                 \
	                                                                                                                   \
	MATLAB_NAMESPACE_END                                                                                               \
                                                                                                                       \
	namespace boost { namespace hana {                                                                                 \
                                                                                                                       \
	template <>                                                                                                        \
	struct tag_of< matlab::vector_kind ## _vector<base_type> > {                                       \
		using type = matlab::vector_kind ## _vector_tag;                                               \
	};                                                                                                                 \
                                                                                                                       \
	/* namespace hana */ } /* namespace boost */ }

_MATLAB_DEF_VEC1(column, real_T)
_MATLAB_DEF_VEC1(column, boolean_T)
_MATLAB_DEF_VEC1(row, real_T)
_MATLAB_DEF_VEC1(row, boolean_T)

#undef _MATLAB_DEF_VEC1

namespace boost { namespace hana {

template <>
struct make_impl<matlab::column_vector_tag>;

template <>
struct make_impl<matlab::row_vector_tag>;

/* namespace hana */ } /* namespace boost */ }

#endif // !MATLAB_DT_VECTOR_HPP
