/* Copyright (c) 2019 Abdullah Güntepe, <abdullah@guentepe.com>
 * Copyright (c) 2019 Jakob Meng, <jakobmeng@web.de>
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

#ifndef HBRS_MPL_FN_MULTIPLY_IMPL_HBRS_MPL_HPP
#define HBRS_MPL_FN_MULTIPLY_IMPL_HBRS_MPL_HPP

#include "../fwd/hbrs_mpl.hpp"

#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/rtsacv.hpp>
#include <hbrs/mpl/dt/rtsarv.hpp>
#include <hbrs/mpl/dt/rtsam.hpp>
#include <hbrs/mpl/dt/submatrix.hpp>
#include <hbrs/mpl/dt/givens_rotation.hpp>
#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/select.hpp>
#include <hbrs/mpl/fn/multiply.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <cmath>

HBRS_MPL_NAMESPACE_BEGIN
namespace hana = boost::hana;
namespace detail {

template<typename Ring>
auto
multiply_impl_rtsacv_scalar::operator()(rtsacv<Ring> lhs, Ring const& rhs) const {
	for (std::size_t i = 0; i < (*size)(lhs); ++i) {
		(*at)(lhs, i) *= rhs;
	}
	return lhs;
}


template<typename Ring>
auto
multiply_impl_scalar_rtsacv::operator()(Ring const& lhs, rtsacv<Ring> rhs) const {
	return multiply(rhs,lhs);
}

template<typename Ring>
decltype(auto)
multiply_impl_rtsarv_rtsacv::operator()(rtsarv<Ring> const& v1, rtsacv<Ring> const& v2) const {
	typedef std::decay_t<Ring> _Ring_;
	_Ring_ sum {0};
	for (std::size_t i {0}; i < v1.length(); ++i)
		sum += v1.at(i) * v2.at(i);
	return sum;
}


template<typename Ring>
decltype(auto)
multiply_impl_rtsacv_rtsarv::operator()(rtsacv<Ring> const& v1, rtsarv<Ring> const& v2) const {
	typedef std::decay_t<Ring> _Ring_;
	//TODO: Implement code to choose best storage_order
	rtsam<_Ring_,storage_order::row_major> result {v1.length(), v2.length()};
	for (std::size_t i {0}; i < (*m)((*size)(result)); ++i) {
		for (std::size_t j {0}; j < (*n)((*size)(result)); ++j) {
			result.at(make_matrix_index(i, j)) = v1.at(i) * v2.at(j);
		}
	}
	return result;
}

template<
	typename Ring,
	typename Matrix1,
	typename Matrix2
>
static auto
multiply_rtsam_rtsam_impl(Matrix1 const& m1, Matrix2 const& m2, hana::basic_type<Ring>) {
	auto m1_sz = (*size)(m1);
	auto m1_m = (*m)(m1_sz);
	auto m1_n = (*n)(m1_sz);
	auto m2_sz = (*size)(m2);
	auto m2_m = (*m)(m2_sz);
	auto m2_n = (*n)(m2_sz);
	
	BOOST_ASSERT(m1_n == m2_m);

	typedef std::decay_t<Ring> _Ring_;
	//TODO: Choose optimal storage order
	rtsam<_Ring_, storage_order::row_major> result {m1_m, m2_n};
	
	for (std::size_t i = 0; i < m1_m; ++i) {
		for (std::size_t j = 0; j < m2_n; ++j) {
			(*at)(result, make_matrix_index(i, j)) =
				select(
					m1,
					std::make_pair(
						i,
						range<std::size_t,std::size_t>(0u, m1_n - 1u)
					)
				) * select(
					m2,
					std::make_pair(
						range<std::size_t,std::size_t>(0u, m2_m - 1u),
						j
					)
				);
		}
	}
	return result;
}

template<
	typename Ring,
	storage_order Order1,
	storage_order Order2
>
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(rtsam<Ring,Order1> const& M1, rtsam<Ring,Order2> const& M2) const {
	return multiply_rtsam_rtsam_impl(M1,M2, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(submatrix<rtsam<Ring,Order>&, Offset,Size> const& M1, submatrix<rtsam<Ring,Order>&, Offset,Size> const& M2) const {
	return multiply_rtsam_rtsam_impl(M1,M2, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order1,
	storage_order Order2,
	typename Offset,
	typename Size
>
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(rtsam<Ring,Order1> const& M1, submatrix<rtsam<Ring,Order2>&, Offset,Size> const& M2) const {
	return multiply_rtsam_rtsam_impl(M1,M2, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
decltype(auto)
multiply_impl_rtsam_rtsam::operator()(submatrix<rtsam<Ring,Order>&, Offset,Size> const& M1, rtsam<Ring,Order> const& M2) const {
	return multiply_rtsam_rtsam_impl(M1,M2, hana::type_c<Ring>);
}

template<
	typename Ring,
	typename RowVector,
	typename Matrix
>
static auto
multiply_rtsarv_rtsam_impl(RowVector const& lhs, Matrix const& rhs, hana::basic_type<Ring>) {
	auto lhs_sz = (*size)(lhs);
	auto rhs_sz = (*size)(rhs);
	auto rhs_m = (*m)(rhs_sz);
	auto rhs_n = (*n)(rhs_sz);
	
	BOOST_ASSERT(lhs_sz == rhs_m);
	
	rtsarv<Ring> result{rhs_n};
	for (std::size_t i = 0; i < rhs_n; ++i) {
		(*at)(result, i) = lhs * select(rhs, std::make_pair(range<std::size_t,std::size_t>(0u, rhs_m - 1u), i));
	}
	return result;
}

template<
	typename Ring,
	storage_order Order
>
decltype(auto)
multiply_impl_rtsarv_rtsam::operator()(rtsarv<Ring> const& v, rtsam<Ring,Order> const& M) const {
	return multiply_rtsarv_rtsam_impl(v,M, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
decltype(auto)
multiply_impl_rtsarv_rtsam::operator()(rtsarv<Ring> const& v, submatrix<rtsam<Ring,Order>&, Offset,Size> const& M) const {
	return multiply_rtsarv_rtsam_impl(v,M, hana::type_c<Ring>);
}

template<
	typename Ring,
	typename Matrix,
	typename ColumnVector
>
auto
multiply_rtsam_rtsacv_impl(Matrix const& lhs, ColumnVector const& rhs, hana::basic_type<Ring>) {
	auto lhs_sz = (*size)(lhs);
	auto lhs_m = (*m)(lhs_sz);
	auto lhs_n = (*n)(lhs_sz);
	auto rhs_sz = (*size)(rhs);
	
	BOOST_ASSERT(lhs_n == rhs_sz);
	
	rtsacv<Ring> result{lhs_m};
	for (std::size_t i = 0; i < lhs_m; ++i) {
		(*at)(result, i) = select(lhs, std::make_pair(i, range<std::size_t,std::size_t>(0u, lhs_n - 1u))) * rhs;
	}
	return result;
}

template<
	typename Ring,
	storage_order Order
>
decltype(auto)
multiply_impl_rtsam_rtsacv::operator()(rtsam<Ring,Order> const& M, rtsacv<Ring> const& v) const {
	return multiply_rtsam_rtsacv_impl(M,v, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
decltype(auto)
multiply_impl_rtsam_rtsacv::operator()(submatrix<rtsam<Ring,Order>&, Offset,Size> const& M, rtsacv<Ring> const& v) const {
	return multiply_rtsam_rtsacv_impl(M,v, hana::type_c<Ring>);
}

template<
	typename Ring,
	storage_order Order
>
auto
multiply_impl_rtsam_scalar::operator()(rtsam<Ring,Order> lhs, Ring const& rhs) const {
	auto lhs_sz = (*size)(lhs);
	auto lhs_m = (*m)(lhs_sz);
	auto lhs_n = (*n)(lhs_sz);
	
	for (std::size_t i = 0; i < lhs_m; ++i) {
		for (std::size_t j = 0; j < lhs_n; ++j) {
			(*at)(lhs, make_matrix_index(i,j)) = (*at)(lhs, make_matrix_index(i,j)) * rhs;
		}
	}
	
	return lhs;
}

template<
	typename Ring,
	storage_order Order
>
auto
multiply_impl_scalar_rtsam::operator()(Ring const& lhs, rtsam<Ring,Order> rhs) const {
	return multiply(rhs,lhs);
}

template<
	typename Ring,
	storage_order Order
>
auto
multiply_impl_givens_rotation_rtsam::operator()(givens_rotation<Ring> const& G, rtsam<Ring,Order> const& A) const {
	return givens_rotation_expression<givens_rotation<Ring> const&, rtsam<Ring,Order> const&>{G,A};
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
auto
multiply_impl_givens_rotation_rtsam::operator()(givens_rotation<Ring> const& G, submatrix<rtsam<Ring,Order>&, Offset, Size> const& A) const {
	return givens_rotation_expression<givens_rotation<Ring> const&, submatrix<rtsam<Ring,Order>&, Offset, Size> const&>{G,A};
}

template<
	typename Ring,
	storage_order Order
>
auto
multiply_impl_rtsam_givens_rotation::operator()(rtsam<Ring,Order> const& A, givens_rotation<Ring> const& G) const {
	return givens_rotation_expression<rtsam<Ring,Order> const&, givens_rotation<Ring> const&>{A,G};
}

template<
	typename Ring,
	storage_order Order,
	typename Offset,
	typename Size
>
auto
multiply_impl_rtsam_givens_rotation::operator()(submatrix<rtsam<Ring,Order>&, Offset, Size> const& A, givens_rotation<Ring> const& G) const {
	return givens_rotation_expression<submatrix<rtsam<Ring,Order>&, Offset, Size> const&, givens_rotation<Ring> const&>{A,G};
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END
#endif // !HBRS_MPL_FN_MULTIPLY_IMPL_HBRS_MPL_HPP
