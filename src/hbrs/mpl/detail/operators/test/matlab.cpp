/* Copyright (c) 2016 Jakob Meng, <jakobmeng@web.de>
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

#define BOOST_TEST_MODULE matlab_test
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <hbrs/mpl/dt/range.hpp>
#include <hbrs/mpl/dt/matrix_index.hpp>
#include <hbrs/mpl/dt/matrix_size.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB
	#include <hbrs/mpl/dt/ml_matrix.hpp>
	#include <hbrs/mpl/dt/ml_vector.hpp>
#endif

#include <hbrs/mpl/detail/test.hpp>

#include <boost/hana/less.hpp>
#include <hbrs/mpl/dt/rtsav.hpp>
#include <hbrs/mpl/dt/ctsav.hpp>
#include <hbrs/mpl/dt/sm.hpp>
#include <hbrs/mpl/dt/srv.hpp>
#include <hbrs/mpl/dt/scv.hpp>

#include <hbrs/mpl/fn/m.hpp>
#include <hbrs/mpl/fn/n.hpp>
#include <hbrs/mpl/fn/size.hpp>
#include <hbrs/mpl/fn/bidiag.hpp>
#include <hbrs/mpl/fn/svd.hpp>
#include <hbrs/mpl/fn/pca.hpp>
#include <hbrs/mpl/fn/pca_filter.hpp>
#include <hbrs/mpl/fn/at.hpp>
#include <hbrs/mpl/fn/transpose.hpp>
#include <hbrs/mpl/fn/vertcat.hpp>
#include <hbrs/mpl/fn/expand.hpp>
#include <hbrs/mpl/fn/mean.hpp>
#include <hbrs/mpl/fn/columns.hpp>
#include <hbrs/mpl/fn/rows.hpp>
#include <hbrs/mpl/fn/select.hpp>

#include <hbrs/mpl/detail/test.hpp>

#ifdef HBRS_MPL_ENABLE_MATLAB

extern "C" {
	#include <hbrs/mpl/detail/matlab_cxn/impl/samples.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/debug1.h>
	#include <hbrs/mpl/detail/matlab_cxn/impl/debug2.h>
}
#undef I /* I is defined by MATLAB Coder, but also used within Boost Unit Test Framework as a template parameter. */

HBRS_MPL_NAMESPACE_BEGIN
namespace detail {

ml_matrix<real_T>
debug1(ml_matrix<real_T> const& a) {
	ml_matrix<real_T> b;
	debug1(&a.data(), &b.data());
	return b;
}

ml_matrix<real_T>
debug2(ml_matrix<real_T> const& a) {
	ml_matrix<real_T> b;
	debug2(&a.data(), &b.data());
	return b;
}

/* namespace detail */ }
HBRS_MPL_NAMESPACE_END

#endif // !HBRS_MPL_ENABLE_MATLAB

namespace utf = boost::unit_test;
namespace tt = boost::test_tools;

#define _TOL 0.000000001

BOOST_AUTO_TEST_SUITE(matlab_test)

#ifdef HBRS_MPL_ENABLE_MATLAB

using hbrs::mpl::detail::environment_fixture;
BOOST_TEST_GLOBAL_FIXTURE(environment_fixture);

BOOST_AUTO_TEST_CASE(matrix_base, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	namespace hana = boost::hana;
	
	ml_matrix<real_T> a0{2,4};
	auto const a0_size = (*size)(a0);
	auto const a0_m = (*m)(a0_size);
	auto const a0_n = (*n)(a0_size);
	{
		cell_0 ds;
		samples(&ds);
		std::copy(ds.f4, ds.f4+8, a0.data().data);
	}
	for(int i = 0; i < a0_m; ++i) {
		for(int j = 0; j < a0_n; ++j) {
			auto correct = i*a0_n+j+1;
			BOOST_TEST(a0[i][j] == correct);
		}
	}
	
	ml_matrix<real_T> a1{4, 5};
	BOOST_TEST((*m)(a1) == 4);
	BOOST_TEST((*n)(a1) == 5);
	(*at)(a1, make_matrix_index(0, 2)) = 3;
	a1[3][4] = 1337;
	
	BOOST_TEST(a1[0][2] == 3);
	BOOST_TEST((*at)(a1, make_matrix_index(0,2)) == 3);
	BOOST_TEST((*size)(a1) == make_matrix_size(4,5));
	BOOST_TEST((*size)(a1[0]) == 5);
	
	smr<ml_matrix<double>&, int> r1 = a1[3];
	BOOST_TEST(r1[4] == 1337);
	BOOST_TEST((*at)(r1,4) == 1337);
	
	ml_matrix<real_T> a2{};
	
	ml_row_vector<real_T> v1{5};
	BOOST_TEST((*size)(v1) == 5);
	
	v1[3] = 1337;
	BOOST_TEST(v1[3] == 1337);
	BOOST_TEST((*at)(v1, 3) == 1337);
	
	smr<ml_matrix<double>, int> v2 = ml_matrix<real_T>{1,1}[0];
	smr<ml_matrix<double>, int> v3 = std::move(a1)[3];
	BOOST_TEST(v3[4] == 1337);
	BOOST_TEST((*at)(v3, 4) == 1337);
	BOOST_TEST((*size)(v3) == 5);
}


BOOST_AUTO_TEST_CASE(matrix_make) {
	using namespace hbrs::mpl;
	
	auto a1 = make_ml_matrix(
		std::initializer_list<double>{
			1.,2.,3.,
			4.,5.,6.
		},
		matrix_size<int, int>{2,3},
		row_major_c
	);
	
	auto a1_m = (*m)(size(a1));
	auto a1_n = (*n)(size(a1));
	
	int nr = 0;
	for(int i = 0; i < a1_m; ++i) {
		for(int j = 0; j < a1_n; ++j) {
			nr += 1;
			BOOST_TEST((*at)(a1, make_matrix_index(i,j)) == nr);
		}
	}
	
	auto a2 = make_ml_matrix(
		std::initializer_list<double>{
			1.,4.,
			2.,5.,
			3.,6.
		},
		matrix_size<int, int>{2,3},
		column_major_c
	);
	
	auto a2_m = (*m)(size(a2));
	auto a2_n = (*n)(size(a2));
	
	int nr2 = 0;
	for(int i = 0; i < a2_m; ++i) {
		for(int j = 0; j < a2_n; ++j) {
			nr2 += 1;
			BOOST_TEST((*at)(a2, make_matrix_index(i,j)) == nr2);
		}
	}
}

BOOST_AUTO_TEST_CASE(matrix_pca, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	namespace hana = boost::hana;
	
	auto const a3 = make_sm(
		make_ctsav(detail::mat_g), 
		make_matrix_size(detail::mat_g_m, detail::mat_g_n), 
		row_major_c
	);
	auto const a3_size = (*size)(a3);
	auto const a3_m = (*m)(a3_size);
	auto const a3_n = (*n)(a3_size);
	BOOST_TEST(a3_m == detail::mat_g_m);
	BOOST_TEST(a3_n == detail::mat_g_n);
	
	{
		cell_0 ds;
		samples(&ds);
		double * a3_ref = ds.f1;
		
		auto const a6 = make_sm(
			make_rtsav(a3_ref, detail::mat_g_m * detail::mat_g_n), 
			make_matrix_size(detail::mat_g_m, detail::mat_g_n), 
			column_major_c
		);
		
		for(std::size_t i = 0; i < a3_m; ++i) {
			for(std::size_t j = 0; j < a3_n; ++j) {
				BOOST_TEST(a6[i][j] == a3[i][j]);
			}
		}
	}
	
	ml_matrix<real_T> a4 = make_ml_matrix(a3);
	auto const a4_size = (*size)(a4);
	auto const a4_m = (*m)(a4_size);
	auto const a4_n = (*n)(a4_size);
	BOOST_TEST(a4_m == a3_m);
	BOOST_TEST(a4_n == a3_n);
	for(std::size_t i = 0; i < a3_m; ++i) {
		for(std::size_t j = 0; j < a3_n; ++j) {
			BOOST_TEST(a4[(int)i][(int)j] == a3[i][j]);
		}
	}
	
	detail::pca_impl_level0_ml_matrix{}(a4, true);
	auto pca1 = (*pca)(a4, true);
	
	auto && pca1_coeff = pca1.coeff();
	auto && pca1_score = pca1.score();
	auto && pca1_latent = pca1.latent();
	auto && pca1_mean = pca1.mean();
	BOOST_TEST((*size)(pca1_coeff) == make_matrix_size((int) a3_n, (int) a3_n));
	BOOST_TEST((*size)(pca1_score) == make_matrix_size((int) a3_m, (int) a3_n));
	BOOST_TEST((*size)(pca1_latent) == (int) a3_n);
	BOOST_TEST((*size)(pca1_mean) == (int) a3_n);
	for(std::size_t i = 0; i < a3_m; ++i) {
		for(std::size_t j = 0; j < a3_n; ++j) {
			BOOST_TEST(a4[(int)i][(int)j] == a3[i][j]);
		}
	}

	static constexpr std::size_t const 
	pca1_c_m = 4, pca1_c_n = 4;
	static constexpr double 
	pca1_c[pca1_c_m*pca1_c_n] = {
      -0.067799985695474,  -0.646018286568728,   0.567314540990512,   0.506179559977706,
      -0.678516235418647,  -0.019993340484099,  -0.543969276583817,   0.493268092159296,
       0.029020832106229,   0.755309622491133,   0.403553469172668,   0.515567418476836,
       0.730873909451461,  -0.108480477171676,  -0.468397518388290,   0.484416225289198
	};
	static constexpr auto pca1_cv = make_ctsav(pca1_c);
	static constexpr auto pca1_cm_sz = make_matrix_size(hana::size_c<pca1_c_m>, hana::size_c<pca1_c_m>);
	static constexpr auto pca1_cm = make_sm(pca1_cv, pca1_cm_sz, row_major_c);
	
	
	
	static constexpr std::size_t const 
	pca1_s_m = 13, pca1_s_n = 4;
	static constexpr double 
	pca1_s[pca1_s_m*pca1_s_n] = {
      36.821825999449707,  -6.870878154227366,  -4.590944457629756,   0.396652582713917,
      29.607273420710953,   4.610881963526309,  -2.247578163663929,  -0.395843536696505,
     -12.981775719737623,  -4.204913183175936,   0.902243082694701,  -1.126100587210616,
      23.714725720918025,  -6.634052554708716,   1.854742000806314,  -0.378564808384689,
      -0.553191676624599,  -4.461732123178686,  -6.087412652325182,   0.142384896047280,
     -10.812490833309822,  -3.646571174544056,   0.912970791674606,  -0.134968810314681,
     -32.588166608817929,   8.979846284936059,  -1.606265913996590,   0.081763927599947,
      22.606395499005593,  10.725906457369444,   3.236537714483414,   0.324334774646370,
      -9.262587237675840,   8.985373347478783,  -0.016909578102172,  -0.543746175981798,
      -3.283969329640687, -14.157277337500910,   7.046512994833767,   0.340509860960603,
       9.220031117829384,  12.386080787220449,   3.428342878284622,   0.435152769664897,
     -25.584908517429557,  -2.781693148152385,  -0.386716066864489,   0.446817950545604,
     -26.903161834677608,  -2.930971165042989,  -2.445522630195304,   0.411607156409657
	};
	static constexpr auto pca1_sv = make_ctsav(pca1_s);
	static constexpr auto pca1_sm_sz = make_matrix_size(hana::size_c<pca1_s_m>, hana::size_c<pca1_s_n>);
	static constexpr auto pca1_sm = make_sm(pca1_sv, pca1_sm_sz, row_major_c);
	
	
	
	static constexpr std::size_t const 
	pca1_l_n = 4;
	static constexpr double 
	pca1_l[pca1_l_n] = {
		5.177968780739056*100,
		0.674964360487231*100,
		0.124054300480810*100,
		0.002371532651878*100
	};
	
	static constexpr auto pca1_lv = make_ctsav(pca1_l);
	static constexpr auto pca1_lcv = make_scv(pca1_lv);
	
	static constexpr std::size_t const 
	pca1_m_n = 4;
	static constexpr double 
	pca1_m[pca1_m_n] = {
		7.461538461538462,  48.153846153846153,  11.769230769230770,  30.000000000000000
	};
	static constexpr auto pca1_mv = make_ctsav(pca1_m);
	static constexpr auto pca1_mrv = make_srv(pca1_mv);
	
	BOOST_TEST((unsigned)(*m)(size(pca1_coeff)) == (unsigned)(*m)(pca1_cm_sz));
	BOOST_TEST((unsigned)(*n)(size(pca1_coeff)) == (unsigned)(*n)(pca1_cm_sz));
	BOOST_TEST((unsigned)(*m)(size(pca1_score)) == (unsigned)(*m)(pca1_sm_sz));
	BOOST_TEST((unsigned)(*n)(size(pca1_score)) == (unsigned)(*n)(pca1_sm_sz));
	BOOST_TEST((unsigned)(*size)(pca1_latent) == pca1_l_n);
	BOOST_TEST((unsigned)(*size)(pca1_mean) == pca1_m_n);
	
	HBRS_MPL_TEST_MMEQ(pca1_coeff, pca1_cm, false);
	HBRS_MPL_TEST_MMEQ(pca1_score, pca1_sm, false);
	HBRS_MPL_TEST_VVEQ(pca1_latent, pca1_lcv, false);
	HBRS_MPL_TEST_VVEQ(pca1_mean, pca1_mrv, false);
	
	ml_matrix<real_T> pca1_cmm{ (int)pca1_c_m, (int)pca1_c_n};
	for(std::size_t i = 0; i < pca1_c_m; ++i) {
		for(std::size_t j = 0; j < pca1_c_n; ++j) {
			pca1_cmm[(int)i][(int)j] = pca1_coeff[(int)i][(int)j];
		}
	}
	
	ml_matrix<real_T> pca1_smm{ (int)pca1_s_m, (int)pca1_s_n};
	for(std::size_t i = 0; i < pca1_s_m; ++i) {
		for(std::size_t j = 0; j < pca1_s_n; ++j) {
			pca1_smm[(int)i][(int)j] = pca1_score[(int)i][(int)j];
		}
	}
	
	ml_row_vector<real_T> pca1_mmrv{(int)pca1_m_n};
	for(std::size_t i = 0; i < pca1_m_n; ++i) {
		pca1_mmrv[(int)i] = pca1_mean[(int)i];
	}
	
	ml_matrix<real_T> r_centered = (*multiply)(pca1_smm, transpose(pca1_cmm));
	ml_matrix<real_T> red2a /*rcst*/ /* reconstructed */ = (*plus)(r_centered, expand(pca1_mmrv, size(r_centered)));
	
	HBRS_MPL_TEST_MMEQ(red2a, a3, false);
}

BOOST_AUTO_TEST_CASE(matrix_pca_filter, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	namespace hana = boost::hana;
	
	auto const a3 = make_sm(
		make_ctsav(detail::mat_g), 
		make_matrix_size(detail::mat_g_m, detail::mat_g_n), 
		row_major_c
	);
	auto const a3_size = (*size)(a3);
	auto const a3_m = (*m)(a3_size);
	auto const a3_n = (*n)(a3_size);
	BOOST_TEST(a3_m == detail::mat_g_m);
	BOOST_TEST(a3_n == detail::mat_g_n);
	
	ml_matrix<real_T> a5{ (int)a3_m, (int)a3_n};
	for(std::size_t i = 0; i < a3_m; ++i) {
		for(std::size_t j = 0; j < a3_n; ++j) {
			a5[(int)i][(int)j] = a3[i][j];
		}
	}
	BOOST_TEST((unsigned) (a5.data().size[0] * a5.data().size[1]) == a3_m * a3_n);
	
	std::vector<bool> const keep_all(std::min(a3_m, a3_n), true);
	auto red1a = (*pca_filter)(a5, keep_all);
	for(std::size_t i = 0; i < a3_m; ++i) {
		for(std::size_t j = 0; j < a3_n; ++j) {
			BOOST_TEST(a5[(int)i][(int)j] == a3[i][j]);
		}
	}
	BOOST_TEST((*size)(red1a.data()) == (*size)(a5));
	for(std::size_t i = 0; i < a3_m; ++i) {
		for(std::size_t j = 0; j < a3_n; ++j) {
			BOOST_TEST(red1a.data()[(int)i][(int)j] == a3[i][j]);
		}
	}
	
	std::vector<bool> keep_first(std::min(a3_m, a3_n), false);
	keep_first[0] = true;
	auto red1b = (*pca_filter)(a5, keep_first);
	
	static constexpr std::size_t const 
	red1b_ref_m = 13, red1b_ref_n = 4;
	static constexpr double 
	red1b_ref[red1b_ref_m*red1b_ref_n] = {
       4.965019185494539,  23.169639395459072,  12.837830799405575,  56.912111921359255,
       5.454165747132270,  28.064830451414771,  12.628458480296437,  51.639183673193358,
       8.341702669638524,  56.962191744251726,  11.392488835627546,  20.511958828093313,
       5.853680396888130,  32.063019733703086,  12.457451842822801,  47.332474299216472,
       7.499044849300464,  48.529195687734408,  11.753176686460884,  29.595686636629370,
       8.194625185369311,  55.490296729562161,  11.455443288107146,  22.097432553750767,
       9.671015691458042,  70.265446280456956,  10.823495057426449,   6.182159268757676,
       5.928825170079655,  32.815039783475825,  12.425287177534420,  46.522424656964127,
       8.089541743755964,  54.438661976590772,  11.500422780136880,  23.230216653964650,
       7.684191535112475,  50.382072660624246,  11.673927246673262,  27.599832497526823,
       6.836420483637805,  41.897905349333783,  12.036803744315502,  36.738680188352085,
       9.196194893040197,  65.513621964622942,  11.026735434693220,  11.300657888908272,
       9.285572449092625,  66.408078242770230,  10.988478626499885,  10.337180933283840
	};
	static constexpr auto red1b_refv = make_ctsav(red1b_ref);
	static constexpr auto red1b_refm_sz = make_matrix_size(hana::size_c<red1b_ref_m>, hana::size_c<red1b_ref_n>);
	static constexpr auto red1b_refm = make_sm(red1b_refv, red1b_refm_sz, row_major_c);
	
	BOOST_TEST((*size)(red1b.data()) == (*size)(a5));
	for(std::size_t i = 0; i < a3_m; ++i) {
		for(std::size_t j = 0; j < a3_n; ++j) {
			BOOST_TEST(red1b.data()[(int)i][(int)j] == red1b_refm[i][j]);
		}
	}
}

BOOST_AUTO_TEST_CASE(matrix_plus, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	
	static constexpr auto a7 = make_sm(
		make_ctsav(detail::mat_i), 
		make_matrix_size(detail::mat_i_m, detail::mat_i_n), 
		row_major_c
	);
	static constexpr auto const a7_size = (*size)(a7);
	static constexpr auto const a7_m = (*m)(a7_size);
	static constexpr auto const a7_n = (*n)(a7_size);
	BOOST_TEST(a7_m == detail::mat_i_m);
	BOOST_TEST(a7_n == detail::mat_i_n);
	
	ml_matrix<real_T> a8{ (int)a7_m, (int)a7_n};
	ml_matrix<real_T> a9{ (int)a7_m, (int)a7_n};
	for(std::size_t i = 0; i < a7_m; ++i) {
		for(std::size_t j = 0; j < a7_n; ++j) {
			a8[(int)i][(int)j] = a7[i][j];
			a9[(int)i][(int)j] = (i+1)*10+(j+1);
		}
	}
	
	ml_matrix<real_T> a10 = (*plus)(a8, a9);
	for(std::size_t i = 0; i < a7_m; ++i) {
		for(std::size_t j = 0; j < a7_n; ++j) {
			BOOST_TEST(a10[(int)i][(int)j] == a7[i][j]+(i+1)*10+(j+1));
		}
	}
	
	ml_matrix<real_T> a11 = (*plus)(a8, 1337);
	for(std::size_t i = 0; i < a7_m; ++i) {
		for(std::size_t j = 0; j < a7_n; ++j) {
			BOOST_TEST(a11[(int)i][(int)j] == a8[(int)i][(int)j]+1337);
		}
	}
	
	ml_matrix<real_T> a12 = detail::debug1(a8);
	for(std::size_t i = 0; i < a7_m; ++i) {
		for(std::size_t j = 0; j < a7_n; ++j) {
			BOOST_TEST(a12[(int)i][(int)j] == a8[(int)i][(int)j] + (i+1)*10+(j+1) );
		}
	}
	
	ml_matrix<real_T> a13 = detail::debug2(a8);
	for(std::size_t i = 0; i < a7_m; ++i) {
		for(std::size_t j = 0; j < a7_n; ++j) {
			BOOST_TEST(a13[(int)i][(int)j] == (i+1)*10 + (j+1));
		}
	}
	
	ml_matrix<real_T> a14{};
	BOOST_TEST((*m)(a14) == 0);
	BOOST_TEST((*n)(a14) == 0);
	
	ml_matrix<real_T> a15{a13}; //copy ctor test
	real_T s1 = a13[0][0];
	a15[0][0] = a13[0][0]+1337;
	BOOST_TEST(a13[0][0] == s1);
	
	a15 = ml_matrix<real_T>{};
}


BOOST_AUTO_TEST_CASE(vertcat_) {
	using namespace hbrs::mpl;
	
	auto const z = make_sm(
		make_ctsav(detail::mat_h), 
		make_matrix_size(detail::mat_h_m, detail::mat_h_n), 
		row_major_c
	);
	
	auto const a = make_ml_matrix(z);
	
	auto a_r0 = a[0];
	auto a_r1 = a[1];
	auto a_r2 = a[2];
	
	detail::vertcat_impl_smr_ml_matrix_smr_ml_matrix{}(a_r0, a_r1);
	detail::vertcat_impl_ml_matrix_smr_ml_matrix{}(detail::vertcat_impl_smr_ml_matrix_smr_ml_matrix{}(a_r0, a_r1), a_r2);
	
	auto b = (*vertcat)(vertcat(a_r0, a_r1), a_r2);
	auto const a_m = (*m)(size(a));
	auto const a_n = (*n)(size(a));
	
	auto const b_m = (*m)(size(b));
	auto const b_n = (*n)(size(b));
	
	BOOST_TEST(a_m == b_m);
	BOOST_TEST(a_n == b_n);
	
	for(int i = 0; i < a_m; ++i) {
		for(int j = 0; j < a_n; ++j) {
			BOOST_TEST((*at)(b, make_matrix_index(i,j)) == (*at)(a, make_matrix_index(i,j)));
		}
	}
	
}

BOOST_AUTO_TEST_CASE(expand_) {
	using namespace hbrs::mpl;
	
	auto const z = make_sm(
		make_ctsav(detail::mat_h), 
		make_matrix_size(detail::mat_h_m, detail::mat_h_n), 
		row_major_c
	);
	
	auto const a = make_ml_matrix(z);
	auto const a_m = (*m)(size(a));
	auto const a_n = (*n)(size(a));
	
	auto a_r0 = a[0];
	auto sz = matrix_size<int,int>{a_m*2, a_n*3};
	
	detail::expand_impl_smr_ml_matrix{}(a_r0, sz);
	
	auto b = (*expand)(a_r0, sz);
	auto const b_m = (*m)(size(b));
	auto const b_n = (*n)(size(b));
	
	BOOST_TEST(a_m*2 == b_m);
	BOOST_TEST(a_n*3 == b_n);
	
	for(int i = 0; i < b_m; ++i) {
		for(int j = 0; j < b_n; ++j) {
			BOOST_TEST((*at)(b, make_matrix_index(i,j)) == (*at)(a, matrix_index<int,int>{0, j%a_n}));
		}
	}
}

BOOST_AUTO_TEST_CASE(mean_) {
	using namespace hbrs::mpl;
	
	auto a = make_ml_matrix(
		std::initializer_list<double>{
			1.,2.,3.,
			4.,5.,6.
		},
		matrix_size<int, int>{2,3},
		row_major_c
	);
	
	auto a_m = (*m)(size(a));
	auto a_n = (*n)(size(a));
	
	auto cm = (*mean)(columns(a));
	auto cm_sz = (*size)(cm);
	
	auto rm = (*mean)(rows(a));
	auto rm_sz = (*size)(rm);
	
	BOOST_TEST(cm_sz == a_n);
	BOOST_TEST(rm_sz == a_m);
	
	for(int i = 0; i < a_m; ++i) {
		double rm_ = 0.;
		for(int j = 0; j < a_n; ++j) {
			rm_ += (*at)(a, make_matrix_index(i,j));
		}
		
		rm_ /= a_n;
		BOOST_TEST((*at)(rm, i) == rm_);
	}
	
	for(int j = 0; j < a_n; ++j) {
		double cm_ = 0.;
		for(int i = 0; i < a_m; ++i) {
			cm_ += (*at)(a, make_matrix_index(i,j));
		}
		
		cm_ /= a_m;
		BOOST_TEST((*at)(cm, j) == cm_);
	}
	
	auto orm = make_ml_matrix(
		std::initializer_list<double>{
			2.0,
			5.0
		},
		matrix_size<int, int>{2,1},
		row_major_c
	);
	
	BOOST_TEST((*m)(size(orm)) == a_m);
	BOOST_TEST((*n)(size(orm)) == 1);
	for(int i = 0; i < a_m; ++i) {
		BOOST_TEST((*at)(rm, i) == (*at)(orm, make_matrix_index((int)i, (int)0)));
	}
	
	auto ocm = make_ml_matrix(
		std::initializer_list<double>{
			2.5, 3.5, 4.5
		},
		matrix_size<int, int>{1,3},
		row_major_c
	);
	
	BOOST_TEST((*m)(size(ocm)) == 1);
	BOOST_TEST((*n)(size(ocm)) == a_n);
	
	for(int j = 0; j < a_n; ++j) {
		BOOST_TEST((*at)(cm, j) == (*at)(ocm, make_matrix_index((int)0, (int)j)));
	}
}


BOOST_AUTO_TEST_CASE(matrix_select) {
	using namespace hbrs::mpl;
	using hbrs::mpl::select;
	
	auto const a = make_ml_matrix(
		std::initializer_list<double>{
			1.,2.,3.,
			4.,5.,6.,
			7.,8.,9.
		},
		make_matrix_size(3,3),
		row_major_c
	);
	
	auto const b = make_ml_matrix(
		std::initializer_list<double>{
			1.,2.,3.,
			4.,5.,6.,
		},
		make_matrix_size(2,3),
		row_major_c
	);
	
	auto const c = make_ml_matrix(
		std::initializer_list<double>{
			4.,5.,6.,
			7.,8.,9.
		},
		make_matrix_size(2,3),
		row_major_c
	);
	
	auto const d = make_ml_matrix(
		std::initializer_list<double>{
			1.,2.,
			4.,5.,
			7.,8.
		},
		make_matrix_size(3,2),
		row_major_c
	);
	
	auto const e = make_ml_matrix(
		std::initializer_list<double>{
			2.,3.,
			5.,6.,
			8.,9.
		},
		make_matrix_size(3,2),
		row_major_c
	);
	
	detail::select_impl_ml_matrix{}(a, make_range(make_matrix_index(0,0), make_matrix_index(1,2)));
	auto rb0 = (*select)(a, make_range(make_matrix_index(0,0), make_matrix_index(1,2)));
	auto rc0 = (*select)(a, make_range(make_matrix_index(1,0), make_matrix_index(2,2)));
	auto rd0 = (*select)(a, make_range(make_matrix_index(0,0), make_matrix_index(2,1)));
	auto re0 = (*select)(a, make_range(make_matrix_index(0,1), make_matrix_index(2,2)));
	
	auto rb1 = (*select)(a, std::make_pair(make_range(0,1), make_range(0,2)));
	auto rc1 = (*select)(a, std::make_pair(make_range(1,2), make_range(0,2)));
	auto rd1 = (*select)(a, std::make_pair(make_range(0,2), make_range(0,1)));
	auto re1 = (*select)(a, std::make_pair(make_range(0,2), make_range(1,2)));
	
	auto rb2 = (*select)(a, std::make_pair(make_matrix_index(0,0), make_matrix_size(2,3)));
	auto rc2 = (*select)(a, std::make_pair(make_matrix_index(1,0), make_matrix_size(2,3)));
	auto rd2 = (*select)(a, std::make_pair(make_matrix_index(0,0), make_matrix_size(3,2)));
	auto re2 = (*select)(a, std::make_pair(make_matrix_index(0,1), make_matrix_size(3,2)));
	
	HBRS_MPL_TEST_MMEQ(b, rb0, false);
	HBRS_MPL_TEST_MMEQ(c, rc0, false);
	HBRS_MPL_TEST_MMEQ(d, rd0, false);
	HBRS_MPL_TEST_MMEQ(e, re0, false);
	
	HBRS_MPL_TEST_MMEQ(b, rb1, false);
	HBRS_MPL_TEST_MMEQ(c, rc1, false);
	HBRS_MPL_TEST_MMEQ(d, rd1, false);
	HBRS_MPL_TEST_MMEQ(e, re1, false);
	
	HBRS_MPL_TEST_MMEQ(b, rb2, false);
	HBRS_MPL_TEST_MMEQ(c, rc2, false);
	HBRS_MPL_TEST_MMEQ(d, rd2, false);
	HBRS_MPL_TEST_MMEQ(e, re2, false);
}

BOOST_AUTO_TEST_CASE(matrix_bidiag, * utf::tolerance(_TOL)) {
	using namespace hbrs::mpl;
	using hbrs::mpl::decompose_mode;
	
	auto make_dataset = [](auto && m, auto && n, auto && ptr) {
		return make_sm(make_rtsav(ptr), make_matrix_size(m, n), row_major_c);
	};
	
	std::vector datasets = {
		make_dataset(detail::mat_a_m, detail::mat_a_n, detail::mat_a),
		make_dataset(detail::mat_g_m, detail::mat_g_n, detail::mat_g),
		make_dataset(detail::mat_j_m, detail::mat_j_n, detail::mat_j)
	};
	
	std::size_t dataset_nr = 0;
	for(auto && dataset : datasets) {
		++dataset_nr;
		//TODO: Reenable other modes once implemented!
		for(auto && mode : { decompose_mode::complete /*, decompose_mode::economy, decompose_mode::zero */ }) {
			BOOST_TEST_MESSAGE(
				"dataset nr := " << dataset_nr << 
				"; mode := " << (mode == decompose_mode::complete ? "complete" : (mode == decompose_mode::economy ? "economy" : "zero"))
			);
			
			auto const a = make_ml_matrix(dataset);
			bidiag_result<ml_matrix<real_T>, ml_matrix<real_T>, ml_matrix<real_T>>
				bg = (*bidiag)(a, mode);
			
			//TODO: Add more diagnostics and checks
			auto && bg_u = (*at)(bg, bidiag_u{});
			auto && bg_b = (*at)(bg, bidiag_b{});
			auto && bg_v = (*at)(bg, bidiag_v{});
			
			auto const ra = (*multiply)(bg_u, multiply(bg_b, transpose(bg_v)));
			
			HBRS_MPL_TEST_MMEQ(a, ra, false);
		}
	}
}

#endif // !HBRS_MPL_ENABLE_MATLAB
BOOST_AUTO_TEST_SUITE_END()
