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

#ifndef ELEMENTAL_FUSE_FN_PCA_HPP
#define ELEMENTAL_FUSE_FN_PCA_HPP

#include <hbrs/mpl/config.hpp>
#include <hbrs/mpl/core/preprocessor.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

#include <hbrs/mpl/dt/pca_result.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/mean.hpp>
#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/minus.hpp>
#include <hbrs/mpl/fn/svd.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/divide.hpp>
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

#include <hbrs/mpl/dt/el_matrix.hpp>
#include <hbrs/mpl/dt/el_vector.hpp>
#include <El.hpp>

ELEMENTAL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;
namespace detail {

struct pca_impl_matrix {
	/* C++ code is equivalent to MATLAB code in file hbrs-mpl/addon/matlab/cxn/pca_level2.m */
	template <typename Ring>
	auto
	operator()(matrix<Ring> const& a, bool economy) const {
		typedef std::decay_t<Ring> _Ring_;
		
		using namespace hbrs::mpl;
		using namespace hana::literals;
		
		auto const a_sz = (*size)(a);
		auto const a_m = (*m)(a_sz);
		auto const a_n = (*n)(a_sz);
		//MATLAB>> [m,n] = size(x);
		
		auto const DOF = a_m-1_c;
		//MATLAB>> DOF=m-1;
		
		row_vector<_Ring_> mu = (*mean)(columns(a));
		//MATLAB>> mu = mean(x);
		
		auto b = (*minus)(a, (*expand)(mu, a_sz));
		//MATLAB>> x = bsxfun(@minus,x,mu);
		
		auto usv = (*svd)(b, economy ? mpl::decompose_mode::economy : mpl::decompose_mode::zero);
		auto && U = (*at)(usv, svd_u{});
		matrix<_Ring_> S{0,0};
		{
			auto && S_ = (*at)(usv, svd_s{});
			
			typedef decltype(S_.at({0,0})) S__Ring;
			typedef std::decay_t<S__Ring> _S__Ring_;
			
			if constexpr (std::is_same_v<_S__Ring_, _Ring_>) {
				S = std::move(S_);
			} else {
				// if Ring := El::Complex<double>, then Ring_of_S_ is Base<El::Complex<double>> a.k.a. double
				El::Copy(S_, S);
			}
		}
		auto && coeff = (*at)(usv, svd_v{});
		//MATLAB>> if Economy
		//MATLAB>> 	[U,S,coeff] = svd(x, 'econ');
		//MATLAB>> else
		//MATLAB>> 	[U,S,coeff] = svd(x, 0);
		//MATLAB>> end
		
		auto Sd = (*diag)(S);
		auto score = (*multiply)(U,S);
		//MATLAB>> S = diag(S);
		//MATLAB>> score =  bsxfun(@times,U,S');
		//MATLAB>> % these two lines are equal to: score =  U*S;
		
		
		std::function<_Ring_(_Ring_)> pow2 = 
			[](_Ring_ e) -> _Ring_ { 
				return (*power)(e, _Ring_(2));
			};
		
		column_vector<_Ring_> latent = (*divide)(transform(std::move(Sd), pow2), _Ring_(DOF));
		//MATLAB>> latent = S.^2./DOF;
		
		using hbrs::mpl::select;
		if (DOF < a_n) {
			if (economy) {
				coeff = (*select)(
					std::move(coeff),
					std::make_pair(El::ALL, El::IR(0, DOF))
				);
				
				score = (*select)(
					std::move(score),
					std::make_pair(El::ALL, El::IR(0, DOF))
				);
				
				latent = (*select)(
					std::move(latent),
					El::IR(0, DOF)
				);
			} else {
				auto score_view = (*select)(score, std::make_pair(El::ALL, El::IR(DOF, a_n)));
				El::Zero(score_view.data());
				
				auto latent_view = (*select)(latent, El::IR(DOF, a_n));
				El::Zero(latent_view.data());
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
		
		
		// Enforce a sign convention on the coefficients -- the largest element in
		// each column will have a positive sign.
		auto mxa = (*transform)(columns(absolute(coeff)), [](auto && column) {
			return fold1(
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
		
		auto colsign = make_row_vector(colsign_seq.data(), colsign_seq.size());
		
		//MATLAB>> [d1, d2] = size(coeff);
		//MATLAB>> colsign = sign(coeff(maxind + (0:d1:(d2-1)*d1)));
		
		matrix<_Ring_> coeff_sgn = (*times)(coeff, expand(colsign, size(coeff)));
		matrix<_Ring_> score_sgn = (*times)(score, expand(colsign, size(score)));
		
		//MATLAB>> coeff = bsxfun(@times, coeff, colsign);
		//MATLAB>> score = bsxfun(@times, score, colsign);
		
		return make_pca_result(coeff_sgn, score_sgn, latent, mu);
	}
};

//TODO: Join with pca_impl_Matrix!
struct pca_impl_dist_matrix {
	template<typename Ring, El::Dist Columnwise, El::Dist Rowwise, El::DistWrap Wrapping>
	auto
	operator()(dist_matrix<Ring, Columnwise, Rowwise, Wrapping> const& a, bool economy) const {
		typedef std::decay_t<Ring> _Ring_;
		static_assert(!std::is_reference<Ring>::value && !std::is_const<Ring>::value, "");
		static_assert(std::is_same<_Ring_, El::Base<_Ring_>>::value, "because S is returned as El::Base<_Ring_>");
		
		using namespace hbrs::mpl;
		using namespace hana::literals;
		
		auto const a_sz = (*size)(a);
		auto const a_m = (*m)(a_sz);
		auto const a_n = (*n)(a_sz);
		//MATLAB>> [m,n] = size(x);
		
		auto const DOF = a_m-1_c;
		//MATLAB>> DOF=m-1;
		
		auto mu = (*mean)(columns(a));
		//MATLAB>> mu = mean(x);
		
		auto b = (*minus)(a, (*expand)(mu, a_sz));
		//MATLAB>> x = bsxfun(@minus,x,mu);
		
		auto usv = (*svd)(b, economy ? mpl::decompose_mode::economy : mpl::decompose_mode::zero);
		
		auto && U = (*at)(usv, svd_u{});
		auto && S = (*at)(usv, svd_s{});
		auto && coeff = (*at)(usv, svd_v{});
		//MATLAB>> if Economy
		//MATLAB>> 	[U,S,coeff] = svd(x, 'econ');
		//MATLAB>> else
		//MATLAB>> 	[U,S,coeff] = svd(x, 0);
		//MATLAB>> end
		
		auto Sd = (*diag)(S);
		auto score = (*multiply)(U,S);
		//MATLAB>> S = diag(S);
		//MATLAB>> score =  bsxfun(@times,U,S');
		//MATLAB>> % these two lines are equal to: score =  U*S;
		
		
		std::function<_Ring_(_Ring_)> pow2 = 
			[](_Ring_ e) -> _Ring_ { 
				return (*power)(e, _Ring_(2));
			};
		
		dist_column_vector latent = (*divide)(transform(std::move(Sd), pow2), _Ring_(DOF));
		//MATLAB>> latent = S.^2./DOF;
		
		using hbrs::mpl::select;
		if (DOF < a_n) {
			if (economy) {
				coeff = (*select)(
					std::move(coeff),
					std::make_pair(El::ALL, El::IR(0, DOF))
				);
				
				score = (*select)(
					std::move(score),
					std::make_pair(El::ALL, El::IR(0, DOF))
				);
				
				latent = (*select)(
					std::move(latent),
					El::IR(0, DOF)
				);
			} else {
				auto score_view = (*select)(score, std::make_pair(El::ALL, El::IR(DOF, a_n)));
				El::Zero(score_view.data());
				
				auto latent_view = (*select)(latent, El::IR(DOF, a_n));
				El::Zero(latent_view.data());
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
		
		
		// Enforce a sign convention on the coefficients -- the largest element in
		// each column will have a positive sign.
		
		
		//MATLAB>> [~,maxind] = max(abs(coeff), [], 1);
		//MATLAB>> [d1, d2] = size(coeff);
		//MATLAB>> colsign = sign(coeff(maxind + (0:d1:(d2-1)*d1)));
		//MATLAB>> coeff = bsxfun(@times, coeff, colsign);
		//MATLAB>> score = bsxfun(@times, score, colsign);
		
		auto coeff_sz = (*size)(coeff);
		auto coeff_m = (*m)(coeff_sz);
		auto coeff_n = (*n)(coeff_sz);
		
		//TODO: Replace this hack!
		dist_row_vector<Ring, El::STAR, El::STAR, El::ELEMENT> colsign{ a.data().Grid(), coeff_n };
		
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
		
		auto coeff_sgn = (*times)(coeff, expand(colsign, size(coeff)));
		auto score_sgn = (*times)(score, expand(colsign, size(score)));
		
		return make_pca_result(coeff_sgn, score_sgn, latent, mu);
	}
};

/* namespace detail */ }
ELEMENTAL_NAMESPACE_END

#define ELEMENTAL_FUSE_FN_PCA_IMPLS boost::hana::make_tuple(                                                           \
		elemental::detail::pca_impl_matrix{},                                                                          \
		elemental::detail::pca_impl_dist_matrix{}                                                                       \
	)

#endif // !ELEMENTAL_FUSE_FN_PCA_HPP