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
#ifndef SGL_FIT
#define SGL_FIT(MODULE) CALL_METHOD(sgl_fit, MODULE, 8)
#endif

extern "C" {
SEXP R_FUN_NAME(sgl_fit, MODULE_NAME)(SEXP r_data, SEXP r_block_dim, SEXP r_blockWeights,
		SEXP r_parameterWeights, SEXP r_alpha, SEXP r_lambda, SEXP r_needed_solutions, SEXP r_config);
}

SEXP FUN_NAME(sgl_fit, MODULE_NAME)(SEXP r_data, SEXP r_block_dim, SEXP r_blockWeights,
		SEXP r_parameterWeights, SEXP r_alpha, SEXP r_lambda_seq, SEXP r_needed_solutions, SEXP r_config) {

	//Start scope timer, note will only be activated if DO_TIMING is defined
	TIMER_START;

	// Configuration
	rList rlist_config(r_config);
	const sgl::AlgorithmConfiguration config(rlist_config);

	//Data and objective
	const rList data_rList(r_data);
	const OBJECTIVE::data_type data(data_rList);
	const OBJECTIVE obj_type(data);

	//Penalty and optimizer
	const sgl::natural_vector block_dim = get_value < sgl::natural_vector > (r_block_dim);
	const sgl::vector blockWeights = get_value < sgl::vector > (r_blockWeights);
	const sgl::matrix parameterWeights = get_value < sgl::matrix > (r_parameterWeights);
	const sgl::numeric alpha = get_value < sgl::numeric > (r_alpha);

	sgl::DimConfig dim_config = sgl::createDimConfig(block_dim, blockWeights, parameterWeights);

	sgl::Interface < OBJECTIVE > sgl_optimizer(obj_type, dim_config, alpha, config);

	// Solve problem
	const sgl::natural_vector needed_solutions = get_value < sgl::natural_vector > (r_needed_solutions);
	const sgl::vector lambda_seq = get_value < sgl::vector > (r_lambda_seq);

	sgl::block_vector_field x_field(needed_solutions.n_elem);
	sgl::vector object_value(needed_solutions.n_elem);
	sgl::vector function_value(needed_solutions.n_elem);

	sgl::natural n_models = sgl_optimizer.optimize(x_field, needed_solutions, object_value, function_value, lambda_seq);

	sgl::sparse_matrix_field beta(n_models);
	for (sgl::natural i = 0; i < n_models; ++i) {
		beta(i) = x_field(i).as_matrix();
	}

	rList res;

	res.attach(rObject(beta), "beta");
	res.attach(rObject(object_value), "loss");
	res.attach(rObject(function_value), "objective");
	res.attach(r_lambda_seq, "lambda");

	return rObject(res);
}

SEXP R_FUN_NAME(sgl_fit, MODULE_NAME)(SEXP r_data, SEXP r_block_dim, SEXP r_blockWeights,
		SEXP r_parameterWeights, SEXP r_alpha, SEXP r_lambda_seq, SEXP r_needed_solutions, SEXP r_config) {

	SGL_TRY {
		return FUN_NAME(sgl_fit, MODULE_NAME)(
			r_data,
			r_block_dim,
			r_blockWeights,
			r_parameterWeights,
			r_alpha,
			r_lambda_seq,
			r_needed_solutions,
			r_config
		);
	} SGL_CATCH_ERROR
}
