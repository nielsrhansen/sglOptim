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
#ifndef SGL_TEST
#define SGL_TEST(MODULE) CALL_METHOD(sgl_test, MODULE, 5)
#endif

extern "C" {
SEXP R_FUN_NAME(sgl_test, MODULE_NAME)(
		SEXP r_data,
		SEXP r_block_dim,
		SEXP r_blockWeights,
		SEXP r_parameterWeights,
		SEXP r_config);
}

SEXP FUN_NAME(sgl_test, MODULE_NAME)(
		SEXP r_data,
		SEXP r_block_dim,
		SEXP r_blockWeights,
		SEXP r_parameterWeights,
		SEXP r_config) {

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

	sgl::DimConfig dim_config = sgl::createDimConfig(block_dim, blockWeights, parameterWeights);

  sgl::SglProblem sgl(dim_config, config);
	sgl::SglTester tester(sgl);
  typename OBJECTIVE::instance_type objective = obj_type.create_instance(sgl.setup);

	sgl::natural max_problems = 10;

	sgl::natural problem_count = tester.test(objective, max_problems);

  return rObject(problem_count);
}

SEXP R_FUN_NAME(sgl_test, MODULE_NAME)(
	SEXP r_data,
  SEXP r_block_dim,
	SEXP r_blockWeights,
	SEXP r_parameterWeights,
	SEXP r_config) {

	SGL_TRY {
		return FUN_NAME(sgl_test, MODULE_NAME)(
			r_data,
      r_block_dim,
			r_blockWeights,
      r_parameterWeights,
			r_config);
	} SGL_CATCH_ERROR
}
