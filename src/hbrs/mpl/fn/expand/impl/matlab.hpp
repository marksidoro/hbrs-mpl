/* Copyright (c) 2016-2018 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_EXPAND_IMPL_MATLAB_HPP
#define HBRS_MPL_FN_EXPAND_IMPL_MATLAB_HPP

#include "../fwd/matlab.hpp"
#ifdef HBRS_MPL_ENABLE_MATLAB

#include <hbrs/mpl/core/preprocessor.hpp>
#include <hbrs/mpl/dt/exception.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/ml_matrix.hpp>
#include <hbrs/mpl/dt/ml_vector.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/at.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<
	typename Matrix,
	typename std::enable_if_t< 
		std::is_same< hana::tag_of_t<Matrix>, ml_matrix_tag >::value
	>*
>
auto
expand_impl_smr_ml_matrix::operator()(
	smr<Matrix, int> const& a,
	matrix_size<int, int> const& sz
) const {
	auto a_sz = (*size)(a);
	auto m_ = (*m)(sz);
	auto n_ = (*n)(sz);
	
	if (((n_ % a_sz) != 0) || (m_ == 0)) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrix_vector_exception{} 
			<< errinfo_ml_vector_size{a_sz}
			<< errinfo_ml_matrix_size{sz}
		));
	}
	
	bool vert_expand = m_ > 1;
	bool horz_expand = n_ != a_sz;
	
	std::decay_t<Matrix> c{m_, n_};
	
	if ((vert_expand && horz_expand) || vert_expand) {
		for(int i = 0; i < m_; ++i) {
			for(int j = 0; j < n_; ++j) {
				(*at)(c, make_matrix_index(i, j)) = (*at)(a, j % a_sz);
			}
		}
	} else {
		//only horz_expand or no expansion
		for(int i = 0; i < n_; ++i) {
			(*at)(c, matrix_index<int, int>(0, i)) = (*at)(a, i % a_sz);
		}
	}
	
	return c;
}

template<typename Ring>
auto
expand_impl_ml_row_vector::operator()(
	ml_row_vector<Ring> const& v,
	matrix_size<int, int> const& sz
) const {
	auto v_sz = (*size)(v);
	auto a_m = (*m)(sz);
	auto a_n = (*n)(sz);
	
	if (((a_n % v_sz) != 0) || (a_m == 0)) {
		BOOST_THROW_EXCEPTION((
			incompatible_matrix_vector_exception{} 
			<< errinfo_ml_vector_size{v_sz}
			<< errinfo_ml_matrix_size{sz}
		));
	}
	
	bool vert_expand = a_m > 1;
	bool horz_expand = a_n != v_sz;
	
	auto c = make_ml_matrix(hana::type_c<std::decay_t<Ring>>, sz);
	
	if ((vert_expand && horz_expand) || vert_expand) {
		for(int i = 0; i < a_m; ++i) {
			for(int j = 0; j < a_n; ++j) {
				(*at)(c, make_matrix_index(i, j)) = (*at)(v, j % v_sz);
			}
		}
	} else {
		//only horz_expand or no expansion
		for(int i = 0; i < a_n; ++i) {
			(*at)(c, make_matrix_index(0, i)) = (*at)(v, i % v_sz);
		}
	}
	
	return c;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB
#endif // !HBRS_MPL_FN_EXPAND_IMPL_MATLAB_HPP
