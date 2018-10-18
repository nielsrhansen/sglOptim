/* Routines for linear sparse group lasso regression.
 Intended for use with R.
 Copyright (C) 2014 Martin Vincent

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

//Uncomment to turn on debuging
//#undef NDEBUG

//Should the timers be activated (only needed for profiling the code)
//#define DO_TIMING

//Configuration
//Debugging
#ifndef NDEBUG
#define SGL_DEBUG
#endif

//Runtime checking for numerical problems
#define SGL_RUNTIME_CHECKS

//Check dimension of input objects
#define SGL_DIM_CHECKS

//Converges checks
#define SGL_CONVERGENCE_CHECK

//Exception handling
#define SGL_CATCH_EXCEPTIONS

//Sgl optimizer
#include <sgl.h>

//Objective
#include "test_objective_linear.h" //linear objective
#include "test_objective_error.h" //linear objective
#include "pkg_c_config.h"

/**********************************
 *
 *  X: dense
 *	Y: dense
 *	W: dense
 *  Hessian: diagonal
 *
 *********************************/

// Module name
#define MODULE linear_test_diagonal_w
#define MATRIX sgl::matrix
#include "create_module.h"

#undef MODULE
#define MODULE linear_test_identity
#include "create_module.h"

#undef MODULE
#define MODULE linear_test_full
#include "create_module.h"

#undef MODULE
#define MODULE linear_test_block_diagonal
#include "create_module.h"

/**********************************
 *
 *  X: dense
 *	Y: sparse
 *	W: dense
 *  Hessian: identity
 *
 *********************************/

 #undef MODULE
 #define MODULE linear_test_diagonal_w_spy
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_identity_spy
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_full_spy
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_block_diagonal_spy
 #include "create_module.h"


/**********************************
 *
 *  X: sparse
 *	Y: dense
 *	W: dense
 *  Hessian: diagonal
 *
 *********************************/

 #undef MATRIX
 #define MATRIX sgl::sparse_matrix

 #undef MODULE
 #define MODULE linear_test_diagonal_w_spx
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_identity_spx
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_full_spx
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_block_diagonal_spx
 #include "create_module.h"


/**********************************
 *
 *  X: sparse
 *	Y: sparse
 *	W: dense
 *  Hessian: diagonal
 *
 *********************************/

 #undef MODULE
 #define MODULE linear_test_diagonal_w_spx_spy
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_identity_spx_spy
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_full_spx_spy
 #include "create_module.h"

 #undef MODULE
 #define MODULE linear_test_block_diagonal_spx_spy
 #include "create_module.h"


/**********************************
 *
 *  X: dense
 *	Y: dense
 *	W: dense
 *  Hessian: diagonal error
 *
 *********************************/
 // Reset macros
 #undef MODULE_NAME
 #undef OBJECTIVE
 #undef PREDICTOR

// Module name
#define MODULE_NAME linear_test_diagonal_error_w

#define OBJECTIVE linear_test_diagonal_error_w

#include <sgl/RInterface/sgl_lambda_seq.h>
#include <sgl/RInterface/sgl_fit.h>
#include <sgl/RInterface/sgl_test.h>

/* **********************************
 *
 *  Registration of methods
 *
 ***********************************/

#include <R_ext/Rdynload.h>

static const R_CallMethodDef sglCallMethods[] = {

	// Core functionality testing
	SGL_LAMBDA(linear_test_diagonal_w),
	SGL_LAMBDA(linear_test_diagonal_w_spx),
	SGL_LAMBDA(linear_test_diagonal_w_spy),
	SGL_LAMBDA(linear_test_diagonal_w_spx_spy),
	SGL_LAMBDA(linear_test_identity),
	SGL_LAMBDA(linear_test_identity_spx),
	SGL_LAMBDA(linear_test_identity_spy),
	SGL_LAMBDA(linear_test_identity_spx_spy),
  SGL_LAMBDA(linear_test_full),
	SGL_LAMBDA(linear_test_full_spx),
	SGL_LAMBDA(linear_test_full_spy),
	SGL_LAMBDA(linear_test_full_spx_spy),
  SGL_LAMBDA(linear_test_block_diagonal),
  SGL_LAMBDA(linear_test_block_diagonal_spx),
  SGL_LAMBDA(linear_test_block_diagonal_spy),
  SGL_LAMBDA(linear_test_block_diagonal_spx_spy),

  SGL_LAMBDA(linear_test_diagonal_error_w),

	SGL_FIT(linear_test_diagonal_w),
	SGL_FIT(linear_test_diagonal_w_spx),
	SGL_FIT(linear_test_diagonal_w_spy),
	SGL_FIT(linear_test_diagonal_w_spx_spy),
	SGL_FIT(linear_test_identity),
	SGL_FIT(linear_test_identity_spx),
	SGL_FIT(linear_test_identity_spy),
	SGL_FIT(linear_test_identity_spx_spy),
  SGL_FIT(linear_test_full),
	SGL_FIT(linear_test_full_spx),
	SGL_FIT(linear_test_full_spy),
	SGL_FIT(linear_test_full_spx_spy),
  SGL_FIT(linear_test_block_diagonal),
  SGL_FIT(linear_test_block_diagonal_spx),
  SGL_FIT(linear_test_block_diagonal_spy),
  SGL_FIT(linear_test_block_diagonal_spx_spy),

  SGL_FIT(linear_test_diagonal_error_w),

	SGL_PREDICT(linear_test_diagonal_w),
	SGL_PREDICT(linear_test_diagonal_w_spx),
	SGL_PREDICT(linear_test_diagonal_w_spy),
	SGL_PREDICT(linear_test_diagonal_w_spx_spy),
	SGL_PREDICT(linear_test_identity),
	SGL_PREDICT(linear_test_identity_spx),
	SGL_PREDICT(linear_test_identity_spy),
	SGL_PREDICT(linear_test_identity_spx_spy),
  SGL_PREDICT(linear_test_full),
	SGL_PREDICT(linear_test_full_spx),
	SGL_PREDICT(linear_test_full_spy),
	SGL_PREDICT(linear_test_full_spx_spy),
  SGL_PREDICT(linear_test_block_diagonal),
  SGL_PREDICT(linear_test_block_diagonal_spx),
  SGL_PREDICT(linear_test_block_diagonal_spy),
  SGL_PREDICT(linear_test_block_diagonal_spx_spy),

	SGL_SUBSAMPLING(linear_test_diagonal_w),
	SGL_SUBSAMPLING(linear_test_diagonal_w_spx),
	SGL_SUBSAMPLING(linear_test_diagonal_w_spy),
	SGL_SUBSAMPLING(linear_test_diagonal_w_spx_spy),
	SGL_SUBSAMPLING(linear_test_identity),
	SGL_SUBSAMPLING(linear_test_identity_spx),
	SGL_SUBSAMPLING(linear_test_identity_spy),
	SGL_SUBSAMPLING(linear_test_identity_spx_spy),
  SGL_SUBSAMPLING(linear_test_full),
  SGL_SUBSAMPLING(linear_test_full_spx),
  SGL_SUBSAMPLING(linear_test_full_spy),
  SGL_SUBSAMPLING(linear_test_full_spx_spy),
  SGL_SUBSAMPLING(linear_test_block_diagonal),
  SGL_SUBSAMPLING(linear_test_block_diagonal_spx),
  SGL_SUBSAMPLING(linear_test_block_diagonal_spy),
  SGL_SUBSAMPLING(linear_test_block_diagonal_spx_spy),

	SGL_TEST(linear_test_diagonal_w),
	SGL_TEST(linear_test_diagonal_w_spx),
	SGL_TEST(linear_test_diagonal_w_spy),
	SGL_TEST(linear_test_diagonal_w_spx_spy),
	SGL_TEST(linear_test_identity),
	SGL_TEST(linear_test_identity_spx),
	SGL_TEST(linear_test_identity_spy),
	SGL_TEST(linear_test_identity_spx_spy),
  SGL_TEST(linear_test_full),
  SGL_TEST(linear_test_full_spx),
  SGL_TEST(linear_test_full_spy),
  SGL_TEST(linear_test_full_spx_spy),
  SGL_TEST(linear_test_block_diagonal),
  SGL_TEST(linear_test_block_diagonal_spx),
  SGL_TEST(linear_test_block_diagonal_spy),
  SGL_TEST(linear_test_block_diagonal_spx_spy),

  SGL_TEST(linear_test_diagonal_error_w),

	// rtools testing
	RTOOLS_TEST(double),
	RTOOLS_TEST(int),
	RTOOLS_TEST(u32),
	RTOOLS_TEST(string),
	RTOOLS_TEST(bool),

	RTOOLS_TEST(mat_double),
  RTOOLS_TEST(mat_u32),
  RTOOLS_TEST(mat_int),
  RTOOLS_TEST(col_double),
	RTOOLS_TEST(col_u32),
	RTOOLS_TEST(col_s32),
	RTOOLS_TEST(sp_mat),

  RTOOLS_TEST(vec_int),
  RTOOLS_TEST(vec_string),

	RTOOLS_TEST(field_double),
	RTOOLS_TEST(field_int),
	RTOOLS_TEST(field_u32),
	RTOOLS_TEST(field_string),
	RTOOLS_TEST(field_bool),
	RTOOLS_TEST(field_mat_double),
	RTOOLS_TEST(field_col_double),
	RTOOLS_TEST(field_col_u32),
	RTOOLS_TEST(field_col_s32),
	RTOOLS_TEST(field_sp_mat),

  {"r_pkg_c_config", (DL_FUNC) &r_pkg_c_config, 0},

  {NULL, NULL, 0}
};

extern "C" {
	void R_init_sglOptim(DllInfo *info);
}

void R_init_sglOptim(DllInfo *info)
{
	// Register the .Call routines.
	R_registerRoutines(info, NULL, sglCallMethods, NULL, NULL);
  R_useDynamicSymbols(info, FALSE);
}
