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

#pragma once

#ifndef MATLAB_DT_MATRIX_HPP
#define MATLAB_DT_MATRIX_HPP

#include <matlab/fwd/dt/matrix.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>

#include <hbrs/mpl/detail/copy_matrix.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>

#include <algorithm>
#include <limits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

#define _MATLAB_DEF_MAT1(base_type)                                                                                    \
	template<>                                                                                                         \
	struct matrix<base_type> {                                                                                         \
		                                                                                                               \
		matrix()                                                                                                       \
		: ptr_{nullptr, nullptr} {                                                                                     \
			emxArray_ ## base_type * emxArray{nullptr};                                                                \
			emxInitArray_ ## base_type(&emxArray, 2);                                                                  \
			ptr_ = {emxArray, emxDestroyArray_ ## base_type};                                                          \
		}                                                                                                              \
		                                                                                                               \
		matrix(int rows, int cols)                                                                                     \
		: ptr_{emxCreate_ ## base_type(rows, cols), emxDestroyArray_ ## base_type} {}                                  \
		                                                                                                               \
		/* This is a reference to a pointer to clarify that matrix's lifetime depends on lifetime of array! */         \
		matrix(base_type*& a, int rows, int cols)                                                                      \
		: ptr_{emxCreateWrapper_ ## base_type(a, rows, cols), emxDestroyArray_ ## base_type} {}                        \
		                                                                                                               \
		matrix(matrix const& rhs) : matrix() {                                                                         \
			if (rhs.ptr_ == nullptr) { return; }                                                                       \
			                                                                                                           \
			auto && a = (*rhs.ptr_);                                                                                   \
			auto && m_ = (*mpl::m)(a);                                                                                 \
			auto && n_ = (*mpl::n)(a);                                                                                 \
			if (a.data != nullptr && m_ > 0 && n_ > 0) {                                                               \
				ptr_ = {                                                                                               \
					emxCreate_ ## base_type(m_, n_),                                                                   \
					emxDestroyArray_ ## base_type                                                                      \
				};                                                                                                     \
                                                                                                                       \
				std::copy(a.data, a.data + (m_*n_), (*ptr_).data);                                                     \
			}                                                                                                          \
		}                                                                                                              \
		                                                                                                               \
		matrix(matrix && rhs) : matrix() {                                                                             \
			swap(*this, rhs);                                                                                          \
		}                                                                                                              \
		                                                                                                               \
		matrix&                                                                                                        \
		operator=(matrix rhs) {                                                                                        \
			swap(*this, rhs);                                                                                          \
			return *this;                                                                                              \
		}                                                                                                              \
		                                                                                                               \
		friend void swap(matrix& lhs, matrix& rhs) noexcept {                                                          \
			using std::swap;                                                                                           \
			swap(lhs.ptr_, rhs.ptr_);                                                                                  \
		}                                                                                                              \
                                                                                                                       \
		decltype(auto)                                                                                                 \
		m() const {                                                                                                    \
			return (*mpl::m)(*ptr_);                                                                                   \
		}                                                                                                              \
		                                                                                                               \
		decltype(auto)                                                                                                 \
		n() const {                                                                                                    \
			return (*mpl::n)(*ptr_);                                                                                   \
		}                                                                                                              \
		                                                                                                               \
		auto                                                                                                           \
		size() const {                                                                                                 \
			return mpl::make_matrix_size(m(), n());                                                                    \
		}                                                                                                              \
		                                                                                                               \
		base_type &                                                                                                    \
		at(mpl::matrix_index<int,int> i) {                                                                             \
			return (*mpl::at)(*ptr_, i);                                                                               \
		}                                                                                                              \
		                                                                                                               \
		template<                                                                                                      \
			typename M,                                                                                                \
			typename N,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				std::is_integral<M>::value &&                                                                          \
				std::is_integral<N>::value &&                                                                          \
				std::numeric_limits<M>::max() <= std::numeric_limits<int>::max() &&                                    \
				std::numeric_limits<N>::max() <= std::numeric_limits<int>::max()                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		base_type &                                                                                                    \
		at(mpl::matrix_index<M, N> const& i) {                                                                         \
			using mpl::m;                                                                                              \
			using mpl::n;                                                                                              \
			using mpl::at;                                                                                             \
			auto && m_ = (*m)(i);                                                                                      \
			auto && n_ = (*n)(i);                                                                                      \
			return (*at)(*ptr_, mpl::matrix_index<int,int>{(int)m_, (int)n_});                                         \
		}                                                                                                              \
                                                                                                                       \
		base_type const&                                                                                               \
		at(mpl::matrix_index<int,int> i) const {                                                                       \
			using mpl::at;                                                                                             \
			return (*at)(*ptr_, i);                                                                                    \
		}                                                                                                              \
		                                                                                                               \
		template<                                                                                                      \
			typename M,                                                                                                \
			typename N,                                                                                                \
			typename std::enable_if_t<                                                                                 \
				std::is_integral<M>::value &&                                                                          \
				std::is_integral<N>::value &&                                                                          \
				std::numeric_limits<M>::max() <= std::numeric_limits<int>::max() &&                                    \
				std::numeric_limits<N>::max() <= std::numeric_limits<int>::max()                                       \
			>* = nullptr                                                                                               \
		>                                                                                                              \
		base_type &                                                                                                    \
		at(mpl::matrix_index<M, N> const& i) const {                                                                   \
			using mpl::m;                                                                                              \
			using mpl::n;                                                                                              \
			using mpl::at;                                                                                             \
			auto && m_ = (*m)(i);                                                                                      \
			auto && n_ = (*n)(i);                                                                                      \
			return (*at)(*ptr_, mpl::matrix_index<int,int>{(int)m_, (int)n_});                                         \
		}                                                                                                              \
		                                                                                                               \
		auto                                                                                                           \
		operator[](int row) & {                                                                                        \
			return mpl::smr<matrix &, int>{*this, row};                                                                \
		}                                                                                                              \
		                                                                                                               \
		auto                                                                                                           \
		operator[](int row) const& {                                                                                   \
			return mpl::smr<matrix const&, int>{*this, row};                                                           \
		}                                                                                                              \
		                                                                                                               \
		auto                                                                                                           \
		operator[](int row) && {                                                                                       \
			return mpl::make_smr(std::move(*this), row);                                                               \
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
	};

_MATLAB_DEF_MAT1(real_T)
_MATLAB_DEF_MAT1(boolean_T)

#undef _MATLAB_DEF_MAT1

MATLAB_NAMESPACE_END

namespace boost { namespace hana {

template <typename BaseType>
struct tag_of< matlab::matrix<BaseType> > {
	using type = matlab::matrix_tag;
};

template <>
struct make_impl<matlab::matrix_tag> {
	template <
		typename T,
		typename M,
		typename N,
		hbrs::mpl::storage_order Order,
		typename std::enable_if_t<
			std::is_same<std::remove_cv_t<T>, real_T>::value &&
			std::is_convertible<M,int>::value && std::is_convertible<N,int>::value
		>* = nullptr
	>
	static auto
	apply(
		hbrs::mpl::sm<hbrs::mpl::rtsav<T>, hbrs::mpl::matrix_size<M, N>, Order> const& b
	) {
		return hbrs::mpl::detail::copy_matrix(b, matlab::matrix<real_T>{(int)b.size().m(), (int)b.size().n()});
	}
	
	template <
		typename T,
		std::size_t Length,
		typename M,
		typename N,
		hbrs::mpl::storage_order Order,
		typename std::enable_if_t<
			std::is_same<std::remove_cv_t<T>, real_T>::value &&
			std::is_convertible<M,int>::value && std::is_convertible<N,int>::value
		>* = nullptr
	>
	static auto
	apply(
		hbrs::mpl::sm<hbrs::mpl::ctsav<T, Length>, hbrs::mpl::matrix_size<M, N>, Order> const& b
	) {
		return hbrs::mpl::detail::copy_matrix(b, matlab::matrix<real_T>{(int)b.size().m(), (int)b.size().n()});
	}
	
	template <
		typename T,
		hbrs::mpl::storage_order Order,
		typename std::enable_if_t<
			std::is_same<std::remove_cv_t<T>, real_T>::value
		>* = nullptr
	>
	static auto
	apply(
		std::initializer_list<T> data,
		hbrs::mpl::matrix_size<int, int> sz,
		hbrs::mpl::storage_order_<Order>
	) {
		return hbrs::mpl::detail::copy_matrix(
			data,
			hbrs::mpl::storage_order_c<Order>, 
			matlab::matrix<real_T>{sz.m(), sz.n()}
		);
	}
};

/* namespace hana */ } /* namespace boost */ }

#endif // !MATLAB_DT_MATRIX_HPP
