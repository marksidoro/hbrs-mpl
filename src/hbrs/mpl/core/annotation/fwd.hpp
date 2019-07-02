/* Copyright (c) 2016-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_CORE_ANNOTATION_FWD_HPP
#define HBRS_MPL_CORE_ANNOTATION_FWD_HPP

#include <hbrs/mpl/config.hpp>
#include <boost/hana/fwd/core/make.hpp>
#include <boost/hana/fwd/core/to.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;

namespace detail {

template <typename Object, typename Annotations>
struct annotated;
struct annotated_tag {};
constexpr auto make_annotated = hana::make<annotated_tag>;
constexpr auto to_annotated = hana::to<annotated_tag>;

/* namespace detail */ }

HBRS_MPL_DEC_CF1(annotations)

HBRS_MPL_DEC_CF1(object)
HBRS_MPL_DEC_CF1(has_annotations)
HBRS_MPL_DEC_CF2(has_annotation)
HBRS_MPL_DEC_CF2(add_annotation)
HBRS_MPL_DEC_CF1(unwrap_if_no_annotations)
HBRS_MPL_DEC_CF2(remove_annotation)

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_CORE_ANNOTATION_FWD_HPP
