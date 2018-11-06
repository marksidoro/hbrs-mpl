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

#ifndef HBRS_MPL_FN_PCA_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_PCA_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/columns.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/mean.hpp>
#include <hbrs/mpl/fn/svd.hpp>
#include <hbrs/mpl/fn/diagonal.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/power.hpp>
#include <hbrs/mpl/fn/maxima.hpp>
#include <hbrs/mpl/fn/absolute.hpp>
#include <hbrs/mpl/fn/signum.hpp>
#include <hbrs/mpl/fn/plus.hpp>
#include <hbrs/mpl/fn/minus.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <hbrs/mpl/fn/less.hpp>
#include <hbrs/mpl/fn/lazy.hpp>
#include <hbrs/mpl/dt/pca_result.hpp>
#include <hbrs/mpl/dt/svd_mode.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <boost/hana/integral_constant.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template <typename Matrix>
constexpr auto
pca_impl_matrix::operator()(Matrix const& a, bool economy) const {
	using hana::literals;
	/* TODO:
	 * implement mean, power, maxima, absolute, signum
	 * implement diagonal, transpose
	 * implement range
	 * implement operator[] for expression
	 * split evaluate in two functions (evaluate and e.g. evaluate_internal) and add evaluated_expression for better error diagnosis
	 */
	auto sz = size(a);
	auto m_ = m(sz);
	auto n_ = n(sz);
	auto DOF = m_ - 1_c;
	auto mu = mean(columns(a));
	auto x = a - mu;
	
	auto svd_ = svd(a, economy ? svd_mode::economy : svd_mode::zero);
	//TODO: svd is expression and will be copied three times here. This means that during evaluation of at calls the function svd will be called three times!?! FIXME!!!
	auto U = at(svd_, svd_u{});
	auto S = at(svd_, svd_s{});
	auto coeff = at(svd_, svd_v{});
	
	auto S_dg = diagonal(S);
	auto score = U * transpose(S_dg);
	// these two lines are equal to: 
	//auto score =  multiply(U, S);
	
	auto latent = power(S_dg, 2_c) / DOF;
	
	if (less(DOF, n_)) {
		if (economy) {
			// When 'economy' value is true, nothing corresponding to zero eigenvalues should be returned.
			coeff[make_range(range::begin,range::end)][make_range(DOF+1_c,range::end)] = [];
			score[make_range(range::begin,range::end)][make_range(DOF+1_c,range::end)] = [];
			latent[make_range(DOF+1_c,range::end)][make_range(range::begin,range::end)] = [];
		} else {
			// otherwise, eigenvalues and corresponding outputs need to pad zeros because svd(x,0) does not return 
			// columns of U corresponding to components of (DOF+1):p.
			score[make_range(range::begin,range::end)][make_range(DOF+1_c,n_)] = 0;
			latent[make_range(DOF+1_c,n_)][1_c] = 0;
		}
	}
	
	// Enforce a sign convention on the coefficients -- the largest element in
	// each column will have a positive sign.
	auto mxa = transform(columns(absolute(coeff)), [](auto && column) {
		return fold1(
			zip(column, indices(column)),
			[](auto && a, auto && b) {
				if (
					(*less)( first(a), first(b) )
				) {
					return HBRS_MPL_FWD(b);
				} else {
					return HBRS_MPL_FWD(a);
				}
			}
		);
	});
	//mxa now contains a sequence of pairs <max_column_value, max_column_index>
	auto col_idxs = transform(mxa, second);
	//idxs now contains a sequence of column indices
	
	auto mat_idxs = (*transform)(
		zip(boost::irange(0, n(size(coeff))), col_idxs), 
		[](auto && pair) {
			return make_matrix_index( (*first)(pair), (*second)(pair) ); 
		}
	);
	// mat_idxs contains a sequence of matrix indices pointing to the largest column values
	
	auto colsign = make_row_vector(
		(*transform)(
			mat_idxs, 
			[&coeff /* NOTE: Probably only works if coeff is evaluated! */](auto && idx) {
				return signum(at(coeff, idx));
			}
		)
	);
	
	auto coeff_sgn = coeff * colsign;
	auto score_sgn = score * colsign;
	
	return make_pca_result(std::move(coeff_sgn), std::move(score_sgn), std::move(latent), std::move(mu));
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_FN_PCA_IMPL_HBRS_MPL_HPP
