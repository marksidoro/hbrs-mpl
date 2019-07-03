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
#ifdef HBRS_MPL_ENABLE_MATLAB

HBRS_MPL_NAMESPACE_BEGIN

#define _HBRS_MPL_DEF_ML_MAT1(base_type)                                                                               \
	smr<ml_matrix<base_type> &, int>                                                                                   \
	ml_matrix<base_type>::operator[](int row) & {                                                                      \
		return smr<ml_matrix &, int>{*this, row};                                                                      \
	}                                                                                                                  \
	                                                                                                                   \
	smr<ml_matrix<base_type> const&, int>                                                                              \
	ml_matrix<base_type>::operator[](int row) const& {                                                                 \
		return smr<ml_matrix const&, int>{*this, row};                                                                 \
	}                                                                                                                  \
	                                                                                                                   \
	smr<ml_matrix<base_type>, int>                                                                                     \
	ml_matrix<base_type>::operator[](int row) && {                                                                     \
		return make_smr(std::move(*this), row);                                                                        \
	}                                                                                                                  \

_HBRS_MPL_DEF_ML_MAT1(real_T)
_HBRS_MPL_DEF_ML_MAT1(boolean_T)

#undef _HBRS_MPL_DEF_ML_MAT1

HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
