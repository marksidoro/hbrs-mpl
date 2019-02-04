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

#ifndef HBRS_MPL_DT_MATRIX_DISTRIBUTION_HPP
#define HBRS_MPL_DT_MATRIX_DISTRIBUTION_HPP

#include <hbrs/mpl/fwd/dt/matrix_distribution.hpp>
#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <hbrs/mpl/detail/is_braces_constructible.hpp>
#include <boost/hana/core/make.hpp>
#include <boost/hana/core/to.hpp>
#include <type_traits>

HBRS_MPL_NAMESPACE_BEGIN

template<typename Columnwise, typename Rowwise, typename Wrapping>
struct matrix_distribution {

    template<typename Columnwise_, typename Rowwise_, typename Wrapping_>
    friend struct matrix_distribution;

    template<
        typename Columnwise_ = Columnwise,
        typename Rowwise_ = Rowwise,
        typename Wrapping_ = Wrapping,
        typename std::enable_if_t<
            std::is_default_constructible<Columnwise_>::value &&
            std::is_default_constructible<Rowwise_>::value &&
            std::is_default_constructible<Wrapping_>::value
        >* = nullptr
    >
    constexpr
    matrix_distribution() {}

    template<typename Columnwise_, typename Rowwise_, typename Wrapping_>
    constexpr
    matrix_distribution(Columnwise_ && rowwise, Rowwise_ && columnwise, Wrapping_ && wrapping)
    : columnwise_{HBRS_MPL_FWD(rowwise)}, rowwise_{HBRS_MPL_FWD(columnwise)}, wrapping_{HBRS_MPL_FWD(wrapping)} {}

    constexpr
    matrix_distribution(matrix_distribution const&) = default;

    template<
        typename Columnwise_,
        typename Rowwise_,
        typename Wrapping_,
        typename std::enable_if_t<
            hbrs::mpl::detail::is_braces_constructible< Columnwise, Columnwise_>::value &&
            hbrs::mpl::detail::is_braces_constructible< Rowwise, Rowwise_>::value &&
            hbrs::mpl::detail::is_braces_constructible< Wrapping, Wrapping_>::value
        >* = nullptr
    >
    constexpr
    matrix_distribution(matrix_distribution<Columnwise_, Rowwise_, Wrapping_> const& rhs)
    : columnwise_{rhs.columnwise_}, rowwise_{rhs.rowwise_}, wrapping_{rhs.wrapping_} {}

    template<
        typename Columnwise_,
        typename Rowwise_,
        typename Wrapping_,
        typename std::enable_if_t<
            hbrs::mpl::detail::is_braces_constructible< Columnwise, Columnwise_>::value &&
            hbrs::mpl::detail::is_braces_constructible< Rowwise, Rowwise_>::value &&
            hbrs::mpl::detail::is_braces_constructible< Wrapping, Wrapping_>::value
        >* = nullptr
    >
    constexpr
    matrix_distribution(matrix_distribution<Columnwise_, Rowwise_, Wrapping_> && rhs)
    : columnwise_{HBRS_MPL_FWD(rhs).columnwise_}, rowwise_{HBRS_MPL_FWD(rhs).rowwise_}, wrapping_{HBRS_MPL_FWD(rhs).wrapping_} {}

    constexpr
    matrix_distribution(matrix_distribution &&) = default;

    matrix_distribution&
    operator=(matrix_distribution const&) = default;
    matrix_distribution&
    operator=(matrix_distribution &&) = default;

    constexpr decltype(auto)
    rowwise() & { return (rowwise_); };

    constexpr decltype(auto)
    rowwise() const& { return (rowwise_); };

    constexpr decltype(auto)
    rowwise() && { return HBRS_MPL_FWD(rowwise_); };

    constexpr decltype(auto)
    columnwise() & { return (columnwise_); };

    constexpr decltype(auto)
    columnwise() const& { return (columnwise_); };

    constexpr decltype(auto)
    columnwise() && { return HBRS_MPL_FWD(columnwise_); };
    
    
    constexpr decltype(auto)
    wrapping() & { return (wrapping_); };

    constexpr decltype(auto)
    wrapping() const& { return (wrapping_); };

    constexpr decltype(auto)
    wrapping() && { return HBRS_MPL_FWD(wrapping_); };

private:
    Columnwise columnwise_;
    Rowwise rowwise_;
    Wrapping wrapping_;
};
HBRS_MPL_NAMESPACE_END

namespace boost { namespace hana {

template <typename Columnwise, typename Rowwise, typename Wrapping>
struct tag_of< hbrs::mpl::matrix_distribution<Columnwise, Rowwise, Wrapping> > {
    using type = hbrs::mpl::matrix_distribution_tag;
};

template <>
struct make_impl<hbrs::mpl::matrix_distribution_tag> {
    template <typename Columnwise, typename Rowwise, typename Wrapping>
    static constexpr hbrs::mpl::matrix_distribution<
        std::decay_t<Columnwise>,
        std::decay_t<Rowwise>,
        std::decay_t<Wrapping>
    >
    apply(Columnwise && columnwise, Rowwise && rowwise, Wrapping && wrapping) {
        return { HBRS_MPL_FWD(columnwise), HBRS_MPL_FWD(rowwise), HBRS_MPL_FWD(wrapping) };
    }
};

/* namespace hana */ } /* namespace boost */ }

#endif // !HBRS_MPL_DT_MATRIX_DISTRIBUTION_HPP
