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

using namespace sgl;

// Registration macro
#ifndef SGL_PREDICT
#define SGL_PREDICT(MODULE) CALL_METHOD(sgl_predict, MODULE, 2)
#endif

extern "C" {
	SEXP R_FUN_NAME(sgl_predict, MODULE_NAME) (SEXP r_data, SEXP r_beta);
}

SEXP FUN_NAME(sgl_predict, MODULE_NAME) (SEXP r_data, SEXP r_beta) {

	//Data and objective
	const rList data_rList(r_data);
	const PREDICTOR::data_type data(data_rList);

	//Parameters
	const sparse_matrix_field beta = get_field < sparse_matrix > (r_beta);

	//Init predictor
	PREDICTOR predictor;

	//Do predictions
	arma::field< arma::field < PREDICTOR::response_type > > responses
		= predictor.predict(data, beta);

	return rObject(responses);
}

SEXP R_FUN_NAME(sgl_predict, MODULE_NAME) (SEXP r_data, SEXP r_beta) {

	SGL_TRY {
		return FUN_NAME(sgl_predict, MODULE_NAME) (r_data, r_beta);
	} SGL_CATCH_ERROR
}
