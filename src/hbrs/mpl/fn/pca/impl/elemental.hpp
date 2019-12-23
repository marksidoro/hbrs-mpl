/* Copyright (c) 2018-2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_PCA_IMPL_ELEMENTAL_HPP
#define HBRS_MPL_FN_PCA_IMPL_ELEMENTAL_HPP

#include "../fwd/elemental.hpp"
#ifdef HBRS_MPL_ENABLE_ELEMENTAL

#include <hbrs/mpl/core/preprocessor.hpp>

#include <hbrs/mpl/dt/pca_control.hpp>
#include <hbrs/mpl/dt/pca_result.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <hbrs/mpl/dt/el_dist_matrix.hpp>
#include <hbrs/mpl/dt/el_dist_vector.hpp>

#include <hbrs/mpl/detail/mpi.hpp>
#include <hbrs/mpl/detail/is_nan.hpp>
#include <hbrs/mpl/detail/is_inf.hpp>
#include <hbrs/mpl/detail/log.hpp>

#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/mean.hpp>
#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/minus.hpp>
#include <hbrs/mpl/fn/svd.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/divide.hpp>
#include <hbrs/mpl/fn/rdivide.hpp>
#include <hbrs/mpl/fn/transform.hpp>
#include <hbrs/mpl/fn/power.hpp>
#include <hbrs/mpl/fn/first.hpp>
#include <hbrs/mpl/fn/less.hpp>
#include <hbrs/mpl/fn/second.hpp>
#include <hbrs/mpl/fn/signum.hpp>
#include <hbrs/mpl/fn/columns.hpp>
#include <hbrs/mpl/fn/absolute.hpp>
#include <hbrs/mpl/fn/indices.hpp>
#include <hbrs/mpl/fn/zip.hpp>
#include <hbrs/mpl/fn/fold1.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/times.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/diag.hpp>
#include <hbrs/mpl/fn/variance.hpp>
#include <hbrs/mpl/fn/any_of.hpp>

#include <boost/numeric/conversion/cast.hpp>
#include <boost/hana/functional/id.hpp>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {
namespace pca_impl_el {

//TODO: Replace ones() with API functions
template<typename Matrix>
static decltype(auto)
ones(Matrix && m) {
	El::Ones(m.data(), m.data().Height(), m.data().Width());
	return HBRS_MPL_FWD(m);
}

//TODO: Replace zeros() with API functions
template<typename Matrix>
static decltype(auto)
zeros(Matrix && m) {
	// el_matrix and el_dist_matrix are zerod by default
	return HBRS_MPL_FWD(m);
}

//TODO: Replace datatype-specific code with calls to generic API functions
template <typename Ring>
static auto
signum_of_largest_element_in_column(el_matrix<Ring> const& coeff) {
	// Returns a row vector that contains signs of largest element in each column
	
	auto mxa = (*transform)(columns(absolute(coeff)), [](auto && column) {
		return (*fold1)(
			zip(column, indices(column)),
			[](auto && a, auto && b) {
				if (
					(*less)( first(a),first(b) )
				) {
					return HBRS_MPL_FWD(b);
				} else {
					return HBRS_MPL_FWD(a);
				}
			}
		);
	});
	// mxa now contains a sequence of pairs <max_value_in_column_value, max_index_in_column>
	auto col_idxs = (*transform)(mxa, second);
	// idxs now contains a sequence of column indices
	
	//MATLAB>> [~,maxind] = max(abs(coeff), [], 1);
	
	auto mat_idxs = (*transform)(
		zip(col_idxs, boost::irange(0, (*n)(size(coeff)))),
		[](auto && pair) {
			return make_matrix_index( (*first)(pair), (*second)(pair) ); 
		}
	);
	// mat_idxs contains a sequence of matrix indices pointing to the largest values per columns
	
	std::vector<int> colsign_seq =
		(*transform)(
			mat_idxs, 
			[&coeff /* NOTE: Probably only works if coeff is evaluated! */](auto && idx) {
				return signum(at(coeff, idx));
			}
		);
	
	auto colsign = make_el_row_vector(colsign_seq.data(), colsign_seq.size());
	
	//MATLAB>> [d1, d2] = size(coeff);
	//MATLAB>> colsign = sign(coeff(maxind + (0:d1:(d2-1)*d1)));
	
	return colsign;
}

template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
static auto
signum_of_largest_element_in_column(el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& coeff) {
	// Returns a row vector that contains signs of largest element in each column
	
	typedef std::decay_t<Ring> _Ring_;
	
	//MATLAB>> [~,maxind] = max(abs(coeff), [], 1);
	//MATLAB>> [d1, d2] = size(coeff);
	//MATLAB>> colsign = sign(coeff(maxind + (0:d1:(d2-1)*d1)));
	
	auto coeff_sz = (*size)(coeff);
	auto coeff_m = (*m)(coeff_sz);
	auto coeff_n = (*n)(coeff_sz);
	
	//TODO: Replace this hack!
	el_dist_row_vector<Ring, El::STAR, El::VC, El::ELEMENT> colsign{ coeff.data().Grid(), coeff_n };
	
	for(El::Int j = 0; j < coeff_n; ++j) {
		El::Int max_idx = 0;
		_Ring_ max_abs = (*absolute)(coeff.data().Get(max_idx, j));
		
		for(El::Int i = 1; i < coeff_m; ++i) {
			_Ring_ pot_abs = (*absolute)(coeff.data().Get(i, j));
			
			if ( (*less)( max_abs, pot_abs) ) {
				max_idx = i;
				max_abs = pot_abs;
			}
		}
		auto sign = (*signum)(coeff.data().Get(max_idx, j));
		colsign.data().Set(0, j, sign);
	}
	
	return colsign;
}

//TODO: Replace this helper function with generic code
template <
	typename Ring,
	typename Size
>
auto
make_row_vector_like(
	el_matrix<Ring> const& a,
	Size && sz
) {
	typedef std::decay_t<Ring> _Ring_;
	return el_row_vector<_Ring_>{HBRS_MPL_FWD(sz)};
}

//TODO: Replace this helper function with generic code
template<
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename Size
>
auto
make_row_vector_like(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	Size && sz
) {
	typedef std::decay_t<Ring> _Ring_;
	return el_dist_row_vector<_Ring_, El::STAR, El::VC>{a.data().Grid(), HBRS_MPL_FWD(sz)};
}

//TODO: Replace this helper function with generic code
template <
	typename Ring,
	typename Size
>
auto
make_column_vector_like(
	el_matrix<Ring> const& a,
	Size && sz
) {
	typedef std::decay_t<Ring> _Ring_;
	return el_column_vector<_Ring_>{HBRS_MPL_FWD(sz)};
}

//TODO: Replace this helper function with generic code
template<
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename Size
>
auto
make_column_vector_like(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	Size && sz
) {
	typedef std::decay_t<Ring> _Ring_;
	return el_dist_column_vector<_Ring_, El::MD, El::STAR>{a.data().Grid(), HBRS_MPL_FWD(sz)};
}

//TODO: Replace this helper function with generic code
template <
	typename Ring,
	typename Size
>
auto
make_matrix_like(
	el_matrix<Ring> const& a,
	Size && sz
) {
	typedef std::decay_t<Ring> _Ring_;
	return el_matrix<_Ring_>{HBRS_MPL_FWD(sz).m(), HBRS_MPL_FWD(sz).n()};
}

//TODO: Replace this helper function with generic code
template<
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename Size
>
auto
make_matrix_like(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	Size && sz
) {
	typedef std::decay_t<Ring> _Ring_;
	return el_dist_matrix<_Ring_, Columnwise, Rowwise, Wrapping>{a.data().Grid(), HBRS_MPL_FWD(sz).m(), HBRS_MPL_FWD(sz).n()};
}

//TODO: Move code to dedicated global function
struct square_t {
	template<typename Ring>
	auto
	operator()(Ring e) const {
		typedef std::decay_t<Ring> _Ring_;
		return (*power)(e, _Ring_(2));
	}
};

constexpr square_t square{};

//TODO: Move code to dedicated global function
template<
	typename Ring,
	typename Predicate,
	typename Value,
	typename std::enable_if_t<
		std::is_constructible_v<Ring, Value>
	>* = nullptr
>
decltype(auto)
replace_if(El::Matrix<Ring> & a, Predicate pred, Value v) {
	for(El::Int j = 0; j < a.Width(); ++j) {
		for(El::Int i = 0; i < a.Height(); ++i) {
			if (pred(a.Get(i,j))) {
				a.Set(i,j, v);
			}
		}
	}
	return a;
}

//TODO: Move code to dedicated global function
template<
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename Predicate,
	typename Value,
	typename std::enable_if_t<
		std::is_constructible_v<Ring, Value>
	>* = nullptr
>
decltype(auto)
replace_if(El::DistMatrix<Ring, Columnwise, Rowwise, Wrapping> & a, Predicate pred, Value v) {
	replace_if(a.Matrix(), pred, HBRS_MPL_FWD(v));
	return a;
}

//TODO: Turn this code into a dedicated, generic pca() implementation
/* C++ code is equivalent to MATLAB code in file src/hbrs/mpl/detail/matlab_cxn/impl/pca_level2.m */
template <typename Matrix, typename Control>
static auto
pca(
	Matrix const& a,
	Control const& ctrl
) {
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:begin";
	HBRS_MPL_LOG_TRIVIAL(trace) << "A:" << loggable{a};
	
	//TODO: Handle complex values
	using namespace hana::literals;
	BOOST_ASSERT(any_of(a, is_nan) == false);
	BOOST_ASSERT(any_of(a, is_inf) == false);
	
	auto const a_sz = (*size)(a);
	auto const a_m = (*m)(a_sz);
	auto const a_n = (*n)(a_sz);
	//MATLAB>> [m,n] = size(x);
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:variance";
	auto vw = ctrl.normalize()
		? (*rdivide)(1., variance(columns(a), 0))
		: ones(make_row_vector_like(a, a_n));
	/* Let x_i be a column of x.
	 * If and only if all entries in x_i are zero, then variance(x_i)=0.
	 * If and only if variance(x_i)=0, then 1/variance(x_i)=inf.
	 * Normalization includes a scalar-multiplication sqrt(1/var(x_i)) * x_i
	 * which will results in a column of NaN's iff x_i has only zeros.
	 * (Sca-)LAPACK's and Elemental's SVD do not handle NaN's and thus we just
	 * replace all inf's in vw with 1's and thus "skipping" normalization.
	 */
	replace_if(vw.data(), is_inf, 1);
	BOOST_ASSERT(any_of(vw, is_inf) == false);
	//MATLAB>> if Normalize
	//MATLAB>>     vVariableWeights = 1./var(x,0);
	//MATLAB>>     % code equals:
	//MATLAB>>     %  vVariableWeights = 1./wnanvar(x,ones(1,m,'like',x),1)
	//MATLAB>> else
	//MATLAB>>     vVariableWeights = ones(1,n,'like',x);
	//MATLAB>> end
	
	auto const DOF = a_m - (ctrl.center() ? 1_c : 0_c);
	//MATLAB>> DOF=m-Center;
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:mean";
	auto mu = ctrl.center() ? (*mean)(columns(a)) : zeros(make_row_vector_like(a, a_n));
	//MATLAB>> if Center
	//MATLAB>> 	mu = wnanmean(x, ones(1,m,'like',x));
	//MATLAB>> else
	//MATLAB>> 	mu = zeros(1,n,'like',x);
	//MATLAB>> end
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:center";
	auto cntr = ctrl.center() ? (*minus)(a, (*expand)(mu, a_sz)) : a;
	BOOST_ASSERT(any_of(cntr, is_nan) == false);
	//MATLAB>> if Center
	//MATLAB>>     x = bsxfun(@minus,x,mu);
	//MATLAB>> end
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:normalize:pre";
	//TODO: Do we need a different algo. for normalizing matrices with complex numbers?
	auto sqrt = [](auto x) { return power(x, 1./2.); };
	auto phi_sqrt = transform(vw, sqrt);
	auto && stdz = ctrl.normalize() ? (*times)(std::move(cntr), expand(phi_sqrt, a_sz)) : std::move(cntr);
	BOOST_ASSERT(any_of(stdz, is_nan) == false);
	//MATLAB>> PhiSqrt = sqrt(vVariableWeights);
	//MATLAB>> if Normalize
	//MATLAB>>     x = bsxfun(@times, x, PhiSqrt);
	//MATLAB>> end
	
	auto usv = (*svd)(stdz, ctrl.economy() ? decompose_mode::economy : decompose_mode::zero);
	auto && U = (*at)(usv, svd_u{});
	//TODO: Make this work for Ring is El::Complex<...>
	auto && S = (*at)(usv, svd_s{});
	
	auto && coeff = (*at)(usv, svd_v{});
	//MATLAB>> if Economy
	//MATLAB>> 	[U,S,coeff] = svd(x, 'econ');
	//MATLAB>> else
	//MATLAB>> 	[U,S,coeff] = svd(x, 0);
	//MATLAB>> end
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:normalize:post";
	coeff = ctrl.normalize()
		? (*times)(coeff, expand( rdivide(1., transpose(phi_sqrt)), size(coeff)) )
		: coeff;
	
	//MATLAB>> if Normalize
	//MATLAB>>     coeff = bsxfun(@times, coeff, 1./PhiSqrt');
	//MATLAB>> end
	
	auto Sd = (*diag)(S);
	auto score = (*multiply)(std::move(U),std::move(S));
	//MATLAB>> S = diag(S);
	//MATLAB>> score =  bsxfun(@times,U,S');
	//MATLAB>> % these two lines are equal to: score =  U*S;
	
	auto latent = (*divide)(transform(std::move(Sd), square), DOF);
	//MATLAB>> latent = S.^2./DOF;
	
	if (DOF < a_n) {
		HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:pad";
		if (ctrl.economy()) {
			// When 'economy' value is true, nothing corresponding to zero eigenvalues should be returned.
			
			// TODO: more generic code is e.g.: coeff[make_range(range::begin,range::end)][make_range(DOF+1_c,range::end)] = [];
			coeff = (*select)(
				std::move(coeff),
				std::make_pair(El::ALL, El::IR(0, DOF))
			);
			BOOST_ASSERT((*equal)(size(coeff), make_matrix_size(a_n, DOF)));
			
			// TODO: more generic code is e.g.: score[make_range(range::begin,range::end)][make_range(DOF+1_c,range::end)] = [];
			score = (*select)(
				std::move(score),
				std::make_pair(El::ALL, El::IR(0, DOF))
			);
			BOOST_ASSERT((*equal)(size(score), make_matrix_size(a_m, DOF)));
			
			// TODO: more generic code is e.g.: latent[make_range(DOF+1_c,range::end)][make_range(range::begin,range::end)] = [];
			latent = (*select)(
				std::move(latent),
				El::IR(0, DOF)
			);
			BOOST_ASSERT((*equal)(size(latent), DOF));
		} else {
			// otherwise, eigenvalues and corresponding outputs need to pad zeros because svd(x,0) does not return 
			// columns of U corresponding to components of (DOF+1):p.
			
			// TODO: more generic code is e.g.: score[make_range(range::begin,range::end)][make_range(DOF+1_c,a_n)] = 0;
			auto score_ = make_matrix_like(a, a_sz);
			auto score_view = score_.data()(El::ALL, El::IR(0,DOF));
			El::Copy(score.data()(El::ALL, El::IR(0,DOF)), score_view);
			score = std::move(score_);
			BOOST_ASSERT((*equal)(size(score), a_sz));
			
			// TODO: more generic code is e.g.: latent[make_range(DOF+1_c,a_n)][1_c] = 0;
			auto latent_ = make_column_vector_like(a, a_n);
			auto latent_view = latent_.data()(El::IR(0,DOF), 0);
			El::Copy(latent.data()(El::IR(0,DOF), 0), latent_view);
			latent = std::move(latent_);
			BOOST_ASSERT((*equal)(size(latent), a_n));
		}
	}
	//MATLAB>> if DOF < n
	//MATLAB>> 	if Economy
	//MATLAB>> 		% When 'Economy' value is true, nothing corresponding to zero
	//MATLAB>> 		% eigenvalues should be returned.
	//MATLAB>> 		coeff(:, DOF+1:end) = [];
	//MATLAB>> 		score(:, DOF+1:end)= [];
	//MATLAB>> 		latent(DOF+1:end, :)= [];
	//MATLAB>> 	else
	//MATLAB>> 		% otherwise, eigenvalues and corresponding outputs need to pad
	//MATLAB>> 		% zeros because svd(x,0) does not return columns of U corresponding
	//MATLAB>> 		% to components of (DOF+1):p.
	//MATLAB>> 		score(:, DOF+1:n) = 0;
	//MATLAB>> 		latent(DOF+1:n, 1) = 0;
	//MATLAB>> 	end
	//MATLAB>> end
	
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:sign";
	// Enforce a sign convention on the coefficients -- the largest element in
	// each column will have a positive sign.
	auto colsign = signum_of_largest_element_in_column(coeff);
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:sign:change";
	auto coeff_sgn = (*times)(std::move(coeff), expand(colsign, size(coeff)));
	auto score_sgn = (*times)(std::move(score), expand(colsign, size(score)));
	
	//MATLAB>> coeff = bsxfun(@times, coeff, colsign);
	//MATLAB>> score = bsxfun(@times, score, colsign);
	
	
	HBRS_MPL_LOG_TRIVIAL(trace) << "coeff:" << loggable{coeff_sgn};
	HBRS_MPL_LOG_TRIVIAL(trace) << "score:" << loggable{score_sgn};
	HBRS_MPL_LOG_TRIVIAL(trace) << "latent:" << loggable{latent};
	HBRS_MPL_LOG_TRIVIAL(trace) << "mu:" << loggable{mu};
	HBRS_MPL_LOG_TRIVIAL(debug) << "pca:elemental:end";
	
	return make_pca_result(coeff_sgn, score_sgn, latent, mu);
}

/* namespace pca_impl_el */ }

template <
	typename Ring,
	typename std::enable_if_t<
		!std::is_reference_v<Ring> &&
		!std::is_const_v<Ring> &&
		std::is_arithmetic_v<Ring>
	>*
>
decltype(auto)
pca_impl_el_matrix::operator()(
	el_matrix<Ring> const& a,
	pca_control<bool,bool,bool> const& ctrl
) const {
	typedef std::decay_t<Ring> _Ring_;
	static_assert(std::is_same<_Ring_, El::Base<_Ring_>>::value, "because S is returned as El::Base<_Ring_>");
	return pca_impl_el::pca(a, ctrl);
}

template<
	typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping,
	typename std::enable_if_t<
		!std::is_reference_v<Ring> &&
		!std::is_const_v<Ring> &&
		std::is_arithmetic_v<Ring>
	>*
>
decltype(auto)
pca_impl_el_dist_matrix::operator()(
	el_dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a,
	pca_control<bool,bool,bool> const& ctrl
) const {
	typedef std::decay_t<Ring> _Ring_;
	static_assert(std::is_same<_Ring_, El::Base<_Ring_>>::value, "because S is returned as El::Base<_Ring_>");
	return pca_impl_el::pca(a, ctrl);
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_ELEMENTAL
#endif // !HBRS_MPL_FN_PCA_IMPL_ELEMENTAL_HPP
