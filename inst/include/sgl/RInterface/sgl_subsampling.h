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
#ifndef SGL_SUBSAMPLING
#define SGL_SUBSAMPLING(MODULE) CALL_METHOD(sgl_subsampling, MODULE, 8)
#endif

extern "C" {
SEXP R_FUN_NAME(sgl_subsampling, MODULE_NAME)(
	SEXP r_data_train,
	SEXP r_data_test,
	SEXP r_block_dim,
	SEXP r_blockWeights,
	SEXP r_parameterWeights,
	SEXP r_alpha,
	SEXP r_lambda_seq,
	SEXP r_config);
}

SEXP FUN_NAME(sgl_subsampling, MODULE_NAME)(
	SEXP r_data_train,
	SEXP r_data_test,
	SEXP r_block_dim,
	SEXP r_blockWeights,
	SEXP r_parameterWeights,
	SEXP r_alpha,
	SEXP r_lambda_seq,
	SEXP r_config) {

	// Configuration
	rList rlist_config(r_config);
	const sgl::AlgorithmConfiguration config(rlist_config);

	const OBJECTIVE::data_type data_train(r_data_train);
	const OBJECTIVE::data_type data_test(r_data_test);
	const OBJECTIVE obj_type(data_train);

	// Penalty parameters
	const sgl::natural_vector block_dim = get_value < sgl::natural_vector > (r_block_dim);
	const sgl::vector blockWeights = get_value < sgl::vector > (r_blockWeights);
	const sgl::matrix parameterWeights = get_value < sgl::matrix > (r_parameterWeights);
	const sgl::numeric alpha = get_value < sgl::numeric > (r_alpha);

	// Create optimizer
	sgl::DimConfig dim_config = sgl::createDimConfig(block_dim, blockWeights, parameterWeights);
	sgl::Interface < OBJECTIVE > sgl_optimizer(obj_type, dim_config, alpha, config);

	// Predictor
	PREDICTOR predictor;

	// Do subsampling
	const sgl::vector lambda_seq = get_value < sgl::vector > (r_lambda_seq);

  boost::tuple<
		arma::field< arma::field< PREDICTOR::response_type > >,
		sgl::natural_vector,
		sgl::natural_vector> response_field
		 = sgl_optimizer.subsamplerun(
			 	predictor,
				lambda_seq,
				data_test);

//Build result R list
rList res;

res.attach(rObject(response_field.get<0>()), "responses");
res.attach(rObject(response_field.get<1>()), "features");
res.attach(rObject(response_field.get<2>()), "parameters");

return rObject(res);

}

SEXP R_FUN_NAME(sgl_subsampling, MODULE_NAME)(
	SEXP r_data_train,
	SEXP r_data_test,
	SEXP r_block_dim,
	SEXP r_blockWeights,
	SEXP r_parameterWeights,
	SEXP r_alpha,
	SEXP r_lambda_seq,
	SEXP r_config) {

	SGL_TRY {
		return FUN_NAME(sgl_subsampling, MODULE_NAME)(
			r_data_train,
			r_data_test,
			r_block_dim,
			r_blockWeights,
			r_parameterWeights,
			r_alpha,
      r_lambda_seq,
			r_config);
	} SGL_CATCH_ERROR

}
