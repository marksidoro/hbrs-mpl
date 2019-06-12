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

#ifndef HBRS_MPL_DETAIL_TEST_FWD_HPP
#define HBRS_MPL_DETAIL_TEST_FWD_HPP

#include <hbrs/mpl/config.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

inline static constexpr std::size_t
mat_a_m = 3, mat_a_n = 3;

inline static constexpr double
mat_a[mat_a_m*mat_a_n] = {
	12.0, -51.0,   4.0,
	06.0, 167.0, -68.0,
	-4.0,  24.0, -41.0
};

inline static constexpr std::size_t
mat_b_m = 5, mat_b_n = 3;

inline static constexpr double
mat_b[mat_b_m*mat_b_n] = {
	12, -51,   4,
	06, 167, -68,
	-4,  24, -41,
	-1,   1,   0,
	02,   0,   3,
};


inline static constexpr std::size_t
mat_c_m = 6, mat_c_n = 5;

inline static constexpr double 
mat_c[mat_c_m*mat_c_n] = {
	+8.79,  +9.93,  +9.83, +5.45,  +3.16,
	+6.11,  +6.91,  +5.04, -0.27,  +7.98,
	-9.15,  -7.93,  +4.86, +4.85,  +3.01,
	+9.57,  +1.64,  +8.83, +0.74,  +5.80,
	-3.49,  +4.02,  +9.80, +10.00, +4.27,
	+9.84,  +0.15,  -8.99, -6.02,  -5.31
};

inline static constexpr std::size_t
mat_d_m = 2, mat_d_n = 2;

inline static constexpr double 
mat_d[mat_d_m*mat_d_n] = {
	00,  1,
	-2, -3
};

inline static constexpr std::size_t
mat_e_m = 4, mat_e_n = 5;

inline static constexpr double 
mat_e[mat_e_m*mat_e_n] = {
	1,0,0,0,2,
	0,0,3,0,0,
	0,2,0,0,0
};

inline static constexpr std::size_t
mat_f_m = 2, mat_f_n = 3;

inline static constexpr double
mat_f[mat_f_m*mat_f_n] = {
	1,2,3,
	-1,2,5
};

inline static constexpr std::size_t
mat_g_m = 13, mat_g_n = 4;

/* This is ingredients matrix from hald.mat used e.g. at https://de.mathworks.com/help/stats/pca.html
 * more sample data sets can be found at https://de.mathworks.com/help/stats/sample-data-sets.html 
 */
inline static constexpr double 
mat_g[mat_g_m*mat_g_n] = {
	07,   26,    6,   60,
	01,   29,   15,   52,
	11,   56,    8,   20,
	11,   31,    8,   47,
	07,   52,    6,   33,
	11,   55,    9,   22,
	03,   71,   17,    6,
	01,   31,   22,   44,
	02,   54,   18,   22,
	21,   47,    4,   26,
	01,   40,   23,   34,
	11,   66,    9,   12,
	10,   68,    8,   12
};

inline static constexpr std::size_t
mat_h_m = 3, mat_h_n = 4;

inline static constexpr double 
mat_h[mat_h_m*mat_h_n] = {
	11, 12, 13, 14,
	21, 22, 23, 24,
	31, 32, 33, 34
};

inline static constexpr std::size_t
mat_i_m = 3, mat_i_n = 4;

inline static constexpr double 
mat_i[mat_i_m*mat_i_n] = {
	11, 21, 31, 
	12, 22, 32,
	13, 23, 33,
	14, 24, 34
};

inline static constexpr std::size_t
mat_j_m = 4, mat_j_n = 13;

/* transpose of mat_g */
inline static constexpr double 
mat_j[mat_j_m*mat_j_n] = {
	07,  1, 11, 11,  7, 11,  3,  1,  2, 21,  1, 11, 10,
	26, 29, 56, 31, 52, 55, 71, 31, 54, 47, 40, 66, 68,
	06, 15,  8,  8,  6,  9, 17, 22, 18,  4, 23,  9,  8,
	60, 52, 20, 47, 33, 22,  6, 44, 22, 26, 34, 12, 12
};

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_DETAIL_TEST_FWD_HPP
