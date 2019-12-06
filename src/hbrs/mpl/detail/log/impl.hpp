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

#ifndef HBRS_MPL_DETAIL_LOG_IMPL_HPP
#define HBRS_MPL_DETAIL_LOG_IMPL_HPP

#include "fwd.hpp"

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/at.hpp>

#include <hbrs/mpl/dt/ctsam.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/rtsacv.hpp>
#include <hbrs/mpl/dt/rtsarv.hpp>

#ifdef HBRS_MPL_ENABLE_ELEMENTAL
	#include <hbrs/mpl/dt/el_matrix.hpp>
	#include <hbrs/mpl/dt/el_vector.hpp>
	#include <hbrs/mpl/dt/el_dist_matrix.hpp>
	#include <hbrs/mpl/dt/el_dist_vector.hpp>
#endif //!HBRS_MPL_ENABLE_ELEMENTAL

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix.hpp>
	#include <hbrs/mpl/dt/ml_vector.hpp>
#endif //!HBRS_MPL_ENABLE_MATLAB

#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/hana/ext/std/vector.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/type.hpp>

#include <sstream>
#include <ostream>
#include <vector>
#include <string>

#define HBRS_MPL_ENABLE_LOGGING /* TODO: Make this a CMake variable? */

#ifdef HBRS_MPL_ENABLE_LOGGING
	#define HBRS_MPL_LOG_TRIVIAL(lvl)                                                                                  \
		BOOST_LOG_TRIVIAL(lvl)                                                                                         \
			<< boost::log::add_value("__FILE__", __FILE__)                                                             \
			<< boost::log::add_value("__LINE__", __LINE__)
#else // !HBRS_MPL_ENABLE_LOGGING
	#define HBRS_MPL_LOG_TRIVIAL(lvl)                                                                                  \
		if constexpr (false)                                                                                           \
			std::clog
#endif //!HBRS_MPL_ENABLE_LOGGING

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<typename T>
struct loggable {
	template<
		typename... dummy,
		typename std::enable_if_t<
			std::is_default_constructible_v<T>
			&& hana::always(hana::true_c)(hana::type_c<dummy>...)
		>* = nullptr
	>
	constexpr
	loggable() {}
	
	template<
		typename... dummy,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<T, T const&>
			&& hana::always(hana::true_c)(hana::type_c<dummy>...)
		>* = nullptr
	>
	constexpr 
	loggable(T const& value)
	: value_{value}
	{}
	
	template<
		typename T_,
		typename std::enable_if_t<
			detail::is_braces_constructible_v<T, T_ &&>
		>* = nullptr
	>
	constexpr
	loggable(T_ && value)
	: value_{HBRS_MPL_FWD(value)}
	{}
	
	constexpr 
	loggable(loggable const&) = default;
	constexpr 
	loggable(loggable &&) = default;
	
	constexpr loggable&
	operator=(loggable const&) = default;
	constexpr loggable&
	operator=(loggable &&) = default;
	
	constexpr decltype(auto)
	value() & { return (value_); };
	
	constexpr decltype(auto)
	value() const& { return (value_); };
	
	constexpr decltype(auto)
	value() && { return HBRS_MPL_FWD(value_); };
	
private:
	T value_;
};

template<typename T>
loggable(T const&) -> loggable<T const&>;

template<typename T>
loggable(T &&) -> loggable<T>;

template<typename T>
loggable(T &) -> loggable<T&>;

/* Ref.: https://github.com/elemental/Elemental/blob/master/src/io/Print.cpp */
template<typename T>
void
configure_precision(std::ostream & os) {
	/* Ref.: https://github.com/elemental/Elemental/blob/master/include/El/core/limits.hpp */
	static auto
	binary_to_decimal_precision = [](int prec) -> int {
		return std::floor(prec*std::log10(2.));
	};
	
	os.precision(
		binary_to_decimal_precision(std::numeric_limits<T>::digits)+1
	);
}

template<
	typename T,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<T>, hana::ext::std::vector_tag >
	>* = nullptr
>
std::ostream &
operator<<(std::ostream & o, loggable<T> const& s) {
	o << '[';
	if (!s.value().empty()) {
		o << "0:" << s.value()[0];
		for(std::size_t i = 1; i < s.value().size(); ++i) {
			o << ", " << i << ':' << s.value()[i];
		}
	}
	o << ']';
	return o;
}

template<
	typename T,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<T>, ctsam_tag > ||
		std::is_same_v< hana::tag_of_t<T>, rtsam_tag >
		
	>* = nullptr
>
std::ostream &
operator<<(std::ostream & lhs, loggable<T> const& rhs) {
	decltype(auto) value = rhs.value();
	
	std::ostringstream msg;
	configure_precision<T>(msg);

	auto sz_ = (*size)(value);
	auto m_ = (*m)(sz_);
	auto n_ = (*m)(sz_);
	
	for(std::size_t i = 0; i < m_; ++i) {
		for(std::size_t j = 0; j < n_; ++j) {
			msg << (*at)(value, make_matrix_index(i,j)) << " ";
		}
		msg << std::endl;
	}
	msg << std::endl;
	
	lhs << msg.str();
	return lhs;
}

template<
	typename T,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<T>, ctsav_tag > ||
		std::is_same_v< hana::tag_of_t<T>, rtsav_tag > ||
		std::is_same_v< hana::tag_of_t<T>, rtsacv_tag > ||
		std::is_same_v< hana::tag_of_t<T>, rtsarv_tag >
	>* = nullptr
>
std::ostream &
operator<<(std::ostream & lhs, loggable<T> const& rhs) {
	decltype(auto) value = rhs.value();
	
	std::ostringstream msg;
	configure_precision<T>(msg);

	auto sz_ = (*size)(value);
	
	for(std::size_t i = 0; i < sz_; ++i) {
		msg << (*at)(value, i) << " ";
	}
	msg << std::endl;
	
	lhs << msg.str();
	return lhs;
}

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

template<typename T>
std::ostream &
operator<<(std::ostream & lhs, loggable<El::Matrix<T>> const& rhs) {
	El::Print(rhs.value(), "", lhs);
	return lhs;
}

template<typename T>
std::ostream &
operator<<(std::ostream & lhs, loggable<El::Matrix<T> const&> const& rhs) {
	El::Print(rhs.value(), "", lhs);
	return lhs;
}

template<typename T>
std::ostream &
operator<<(std::ostream & lhs, loggable<El::Matrix<T> &> const& rhs) {
	El::Print(rhs.value(), "", lhs);
	return lhs;
}

template<typename Ring>
std::ostream &
operator<<(std::ostream & lhs, loggable<El::AbstractDistMatrix<Ring> const&> const& rhs) {
	El::Print(rhs.value(), "", lhs);
	return lhs;
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
std::ostream &
operator<<(std::ostream & lhs, loggable<El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping>> const& rhs) {
	El::Print(rhs.value(), "", lhs);
	return lhs;
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
std::ostream &
operator<<(std::ostream & lhs, loggable<El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> const&> const& rhs) {
	El::Print(rhs.value(), "", lhs);
	return lhs;
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
std::ostream &
operator<<(std::ostream & lhs, loggable<El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> &> const& rhs) {
	El::Print(rhs.value(), "", lhs);
	return lhs;
}

template<
	typename T,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<T>, el_matrix_tag > ||
		std::is_same_v< hana::tag_of_t<T>, el_dist_matrix_tag > ||
		std::is_same_v< hana::tag_of_t<T>, el_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<T>, el_row_vector_tag > ||
		std::is_same_v< hana::tag_of_t<T>, el_dist_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<T>, el_dist_row_vector_tag >
	>* = nullptr
>
std::ostream &
operator<<(std::ostream & lhs, loggable<T> const& rhs) {
	lhs << loggable{rhs.value().data()};
	return lhs;
}

#endif //!HBRS_MPL_ENABLE_ELEMENTAL

#ifdef HBRS_MPL_ENABLE_MATLAB

template<
	typename T,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<T>, ml_matrix_tag >
	>* = nullptr
>
std::ostream &
operator<<(std::ostream & lhs, loggable<T> const& rhs) {
	decltype(auto) value = rhs.value();
	
	std::ostringstream msg;
	configure_precision<T>(msg);

	auto sz_ = (*size)(value);
	auto m_ = (*m)(sz_);
	auto n_ = (*m)(sz_);
	
	for(int i = 0; i < m_; ++i) {
		for(int j = 0; j < n_; ++j) {
			msg << (*at)(value, make_matrix_index(i,j)) << " ";
		}
		msg << std::endl;
	}
	msg << std::endl;
	
	lhs << msg.str();
	return lhs;
}

template<
	typename T,
	typename std::enable_if_t<
		std::is_same_v< hana::tag_of_t<T>, ml_column_vector_tag > ||
		std::is_same_v< hana::tag_of_t<T>, ml_row_vector_tag >
	>* = nullptr
>
std::ostream &
operator<<(std::ostream & lhs, loggable<T> const& rhs) {
	decltype(auto) value = rhs.value();
	
	std::ostringstream msg;
	configure_precision<T>(msg);

	auto sz_ = (*size)(value);
	
	for(int i = 0; i < sz_; ++i) {
		msg << (*at)(value, i) << " ";
	}
	msg << std::endl;
	
	lhs << msg.str();
	return lhs;
}

#endif //!HBRS_MPL_ENABLE_MATLAB

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_LOG_IMPL_HPP
