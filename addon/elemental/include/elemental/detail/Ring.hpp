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

#ifndef ELEMENTAL_DETAIL_RING_HPP
#define ELEMENTAL_DETAIL_RING_HPP

#include <elemental/config.hpp>
#include <El.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace detail {

template<typename Matrix>
struct Ring;

template<typename Ring_>
struct Ring<El::Matrix<Ring_>> {
	typedef Ring_ type;
};

template<typename Ring_>
struct Ring<El::AbstractDistMatrix<Ring_>> {
	typedef Ring_ type;
};

template<typename Ring_, El::Dist U, El::Dist V, El::DistWrap wrap>
struct Ring<El::DistMatrix<Ring_, U, V, wrap>> {
	typedef Ring_ type;
};

template<typename Matrix>
using Ring_t = typename Ring<Matrix>::type;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#endif // !ELEMENTAL_DETAIL_RING_HPP
