/*
	Sgl template library for optimizing sparse group lasso penalized objectives.
    Copyright (C) 2012 Martin Vincent

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

#ifndef SGL_H_
#define SGL_H_

#define R_NO_REMAP

//Progress monitor
#include <progress.hpp>

//Rcpp ect
#include <RcppCommon.h>
#include <Rconfig.h>
#include <RcppArmadilloConfig.h>

// Debugging
#ifdef SGL_DEBUG
// Do debugging
#ifdef ARMA_NO_DEBUG
#undef ARMA_NO_DEBUG
#endif
#ifdef NDEBUG
#undef NDEBUG
#endif
//#define SGL_DEBUG_SIMPLE
//#define SGL_DEBUG_COMPLEX
//#define SGL_DEBUG_INFO_ALL
//#define SGL_DEBUG_INFO_STEPSIZE
#else
// Do no debugging
#define ARMA_NO_DEBUG
#ifndef NDEBUG
#define NDEBUG
#endif
#endif

// Registration helper macros
#define STR_VALUE(x) #x
#define GET_STR_VALUE(x) STR_VALUE(x)

#define CREATE_FUN_NAME(METHOD, MODULE) MODULE ## _ ## METHOD
#define CREATE_R_FUN_NAME(METHOD, MODULE) r_ ## MODULE ## _ ## METHOD
#define FUN_NAME(METHOD, MODULE) CREATE_FUN_NAME(METHOD, MODULE)
#define R_FUN_NAME(METHOD, MODULE) CREATE_R_FUN_NAME(METHOD, MODULE)

#define CALL_METHOD(METHOD, MODULE, ARGS) {GET_STR_VALUE(FUN_NAME(METHOD,MODULE)), (DL_FUNC) &r_ ## MODULE ## _ ## METHOD, ARGS}

//Support for xl matrices
//#define ARMA_64BIT_WORD

#include <armadillo>
#include <Rcpp.h>

//Boost
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
using boost::tuple;

#include <limits>
#include <time.h>
#include <stdexcept>

#include "sgl/simple_timer.h"

//Arma additions
#include <sgl/arma_additions.h>

//R tools
#include <rtools.h>

//sgl
namespace sgl {
#include "sgl/block_vector.h"
#include "sgl/numeric.h"
#include "sgl/error.h"
#include "sgl/algorithm_config.h"
#include "sgl/dim_config.h"
#include "sgl/sgl_problem.h"
#include "sgl/sgl_tester.h"
#include "sgl/sgl_optimizer.h"
#include "sgl/objective_function_type.h"
#include "sgl/interface_basic.h"
#include "sgl/objective/sgl_matrix_data.h"
#include "sgl/objective/sgl_gl_loss_base.h"
#include "sgl/objective/sgl_gl_loss_dense.h"
#include "sgl/objective/sgl_gl_loss_sparse.h"
#include "sgl/objective/linear_response.h"
#include "sgl/objective/linear_predictor.h"
}


#endif /* SGL_H_ */
