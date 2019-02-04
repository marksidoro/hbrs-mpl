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

#include <hbrs/mpl/fn/sum.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace mpl = hbrs::mpl;

namespace detail {

//TODO: Add non-dist impls

template auto sum_impl_DistMatrix_columns::operator()(columns_expr<El::DistMatrix<float>> const&) const;
template auto sum_impl_DistMatrix_columns::operator()(columns_expr<El::DistMatrix<El::Complex<float>>> const&) const;
template auto sum_impl_DistMatrix_columns::operator()(columns_expr<El::DistMatrix<double>> const&) const;
template auto sum_impl_DistMatrix_columns::operator()(columns_expr<El::DistMatrix<El::Complex<double>>> const&) const;

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END
