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

//This objective contains errors

using namespace sgl;

#ifndef FOBENIUS_NORM_WEIGHTED_ERROR_HPP_
#define FOBENIUS_NORM_WEIGHTED_ERROR_HPP_

//type_X : sgl::matrix or sgl::sparse_matrix
//type_Y : sgl::matrix or sgl::sparse_matrix

template < typename type_X, typename type_Y, typename type_H >
class FrobeniusLossWeightedError {

public:

	const natural n_variables; // number of parameters

private:

	type_Y const& Y; //response - matrix of size n_samples x n_responses
	matrix const& W; //vector of size n_samples x n_responses

	matrix lp; //linear predictors - matrix of size n_samples x n_responses

public:

	typedef type_H hessian_type;
	typedef typename type_H::representation hessian_representation;

	typedef DataPackage_4<
		MatrixData<type_X>,
		MultiResponse<type_Y, 'Y'>,
		Data<sgl::matrix, 'W'>,
		Data<sgl::natural_matrix, 'D'> > data_type; //D for dummy

	FrobeniusLossWeightedError() 	:
		n_variables(0),
		Y(null_matrix),
		W(null_matrix),
		lp(null_matrix)	{}

	FrobeniusLossWeightedError(data_type const& data) :
		n_variables(data.get_B().n_responses),
		Y(data.get_B().response),
		W(data.get_C().data),
		lp(data.get_A().n_samples, n_variables) {}

	void set_lp(matrix const& lp) {
		this->lp = lp;
	}

	void set_lp_zero() {
		lp.zeros();
	}

	const matrix gradients() const {
		return -2*trans(W%(lp-Y));
	}

	void compute_hessians() const	{
		return;
	}

  const hessian_representation hessians(natural i) const {
		return hessians(i, hessian_type());
	}

	const hessian_representation hessians(natural i, hessian_identity<false> ) const {
		return -2 * W(i,0);
	}

	const hessian_representation hessians(natural i, hessian_diagonal<false> ) const {
		return -2 * trans(W.row(i));
	}


	const numeric sum_values() const {
		return accu(W%(lp-Y)%(lp-Y));
	}

};

// dense dense
typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeightedError < matrix, matrix,
		 	hessian_diagonal<false> > > > linear_test_diagonal_error_w
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeightedError < matrix, matrix,
		 	hessian_identity<false> > > > linear_test_identity_error
;

#endif /* TEST_OBJECTIVE_ERROR_H_ */
