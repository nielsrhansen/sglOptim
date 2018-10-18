/*
	Sgl template library for optimizing sparse group lasso penalized objectives.
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

// Registration macro
#ifndef SGL_LAMBDA
#define SGL_LAMBDA(MODULE) CALL_METHOD(sgl_lambda, MODULE, 9)
#endif

extern "C" {
SEXP R_FUN_NAME(sgl_lambda, MODULE_NAME) (SEXP r_data, SEXP r_block_dim, SEXP r_blockWeights,
		SEXP r_parameterWeights, SEXP r_alpha, SEXP r_numberOfModels, SEXP r_lambdaMin, SEXP r_lambdaMinRel, SEXP r_config);
}

SEXP FUN_NAME(sgl_lambda, MODULE_NAME) (SEXP r_data, SEXP r_block_dim, SEXP r_blockWeights,
		SEXP r_parameterWeights, SEXP r_alpha, SEXP r_numberOfModels, SEXP r_lambdaMin, SEXP r_lambdaMinRel, SEXP r_config) {

	TIMER_START;

	// Configuration
	rList rlist_config(r_config);
	const sgl::AlgorithmConfiguration config(rlist_config);


	//Data and objective
	const rList data_rList(r_data);
	const OBJECTIVE::data_type data(data_rList);
	const OBJECTIVE obj_type(data);

	//Penalty and otimizer
	const sgl::natural_vector block_dim = get_value < sgl::natural_vector > (r_block_dim);
	const sgl::vector blockWeights = get_value < sgl::vector > (r_blockWeights);
	const sgl::matrix parameterWeights = get_value < sgl::matrix > (r_parameterWeights);
	const sgl::numeric alpha = get_value < sgl::numeric > (r_alpha);
	const bool lambda_min_rel = get_value<bool>(r_lambdaMinRel);

	sgl::DimConfig dim_config = sgl::createDimConfig(block_dim, blockWeights, parameterWeights);
	sgl::Interface < OBJECTIVE > sgl_optimizer(obj_type, dim_config, alpha, config);

	//Compute lambda max
	sgl::numeric lambda_max = sgl_optimizer.lambda_max();
 	sgl::numeric lambda_min;

  //Get lambda min
	if(lambda_min_rel) {
			lambda_min = lambda_max*get_value < sgl::numeric > (r_lambdaMin);
	}

	else {
			lambda_min = get_value < sgl::numeric > (r_lambdaMin);
	}

	//Make lambda sequence
	sgl::vector result = sgl_optimizer.lambda_sequence(lambda_max, lambda_min, get_value < sgl::natural > (r_numberOfModels));

	return rObject(result);
}

SEXP R_FUN_NAME(sgl_lambda, MODULE_NAME) (SEXP r_data, SEXP r_block_dim, SEXP r_blockWeights,
		SEXP r_parameterWeights, SEXP r_alpha, SEXP r_numberOfModels, SEXP r_lambdaMin, SEXP r_lambdaMinRel, SEXP r_config) {

	SGL_TRY {
		return FUN_NAME(sgl_lambda, MODULE_NAME) (
			r_data,
			r_block_dim,
			r_blockWeights,
			r_parameterWeights,
			r_alpha,
			r_numberOfModels,
			r_lambdaMin,
			r_lambdaMinRel,
			r_config
		);
	} SGL_CATCH_ERROR
}
