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

#ifndef MATLAB_FUSE_FN_EXPAND_HPP
#define MATLAB_FUSE_FN_EXPAND_HPP

#include <matlab/config.hpp>
#include <hbrs/mpl/preprocessor/core.hpp>
#include <matlab/dt/exception.hpp>
#include <hbrs/mpl/dt/smr.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <matlab/dt/matrix.hpp>
#include <matlab/dt/vector.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/tag_of.hpp>
#include <type_traits>

MATLAB_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace mpl = hbrs::mpl;

namespace detail {

//TODO: Join with elemental::detail::expand_impl_smr!
struct expand_impl_smr {
	template<
		typename Matrix,
		typename std::enable_if_t< 
			std::is_same< hana::tag_of_t<Matrix>, matlab::matrix_tag >::value
		>* = nullptr
	>
	auto
	operator()(
		mpl::smr<Matrix, int> const& a,
		mpl::matrix_size<int, int> const& sz
	) const {
		using namespace hbrs::mpl;
		auto a_sz = (*size)(a);
		auto m_ = (*m)(sz);
		auto n_ = (*n)(sz);
		
		if (((n_ % a_sz) != 0) || (m_ == 0)) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrix_vector_exception{} 
				<< matlab::errinfo_vector_size{a_sz}
				<< matlab::errinfo_matrix_size{sz}
			));
		}
		
		bool vert_expand = m_ > 1;
		bool horz_expand = n_ != a_sz;
		
		std::decay_t<Matrix> c{m_, n_};
		
		if ((vert_expand && horz_expand) || vert_expand) {
			for(int i = 0; i < m_; ++i) {
				for(int j = 0; j < n_; ++j) {
					(*at)(c, mpl::make_matrix_index(i, j)) = (*at)(a, j % a_sz);
				}
			}
		} else {
			//only horz_expand or no expansion
			for(int i = 0; i < n_; ++i) {
				(*at)(c, mpl::matrix_index<int, int>(0, i)) = (*at)(a, i % a_sz);
			}
		}
		
		return c;
	}
};

//TODO: Join with elemental::detail::expand_impl_row_vector!
struct expand_impl_row_vector {
	template<typename Ring>
	auto
	operator()(
		row_vector<Ring> const& v,
		mpl::matrix_size<int, int> const& sz
	) const {
		using namespace hbrs::mpl;
		
		auto v_sz = (*size)(v);
		auto a_m = (*m)(sz);
		auto a_n = (*n)(sz);
		
		if (((a_n % v_sz) != 0) || (a_m == 0)) {
			BOOST_THROW_EXCEPTION((
				incompatible_matrix_vector_exception{} 
				<< matlab::errinfo_vector_size{v_sz}
				<< matlab::errinfo_matrix_size{sz}
			));
		}
		
		bool vert_expand = a_m > 1;
		bool horz_expand = a_n != v_sz;
		
		auto c = make_matrix(hana::type_c<std::decay_t<Ring>>, sz);
		
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
};

/* namespace detail */ }
MATLAB_NAMESPACE_END

#define MATLAB_FUSE_FN_EXPAND_IMPLS boost::hana::make_tuple(                                                           \
		matlab::detail::expand_impl_smr{},                                                                             \
		matlab::detail::expand_impl_row_vector{}                                                                       \
	)

#endif // !MATLAB_FUSE_FN_EXPAND_HPP
