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

#include "impl.hpp"

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ctsam<double, 3, 3, storage_order::row_major>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<rtsam<double, storage_order::row_major>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ctsav<double, 3>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<rtsav<double>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<rtsacv<double>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<rtsarv<double>> const& rhs);

#ifdef HBRS_MPL_ENABLE_ELEMENTAL

template
std::ostream &
operator<<(std::ostream & lhs, loggable<El::Matrix<double>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<el_matrix<double>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<El::AbstractDistMatrix<double> const&> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<El::DistMatrix<double>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<el_dist_matrix<double>> const& rhs);

#endif //!HBRS_MPL_ENABLE_ELEMENTAL

#ifdef HBRS_MPL_ENABLE_MATLAB

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_matrix<real_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_matrix<creal_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_matrix<boolean_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_column_vector<real_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_column_vector<creal_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_column_vector<boolean_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_row_vector<real_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_row_vector<creal_T>> const& rhs);

template
std::ostream &
operator<<(std::ostream & lhs, loggable<ml_row_vector<boolean_T>> const& rhs);

#endif //!HBRS_MPL_ENABLE_MATLAB

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END
