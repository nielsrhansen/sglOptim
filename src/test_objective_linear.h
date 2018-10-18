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

#ifndef FOBENIUS_NORM_WEIGHTED_HPP_
#define FOBENIUS_NORM_WEIGHTED_HPP_

//type_X : sgl::matrix or sgl::sparse_matrix
//type_Y : sgl::matrix or sgl::sparse_matrix

template < typename type_X, typename type_Y, typename type_H >
class FrobeniusLossWeighted {

public:

	const natural n_variables; // number of parameters

private:

	type_Y const& Y; //response - matrix of size n_samples x n_responses
	matrix const& W; //vector of size n_samples x n_responses

	matrix lp; //linear predictors - matrix of size n_samples x n_responses

public:

	typedef type_H hessian_type;
	typedef typename type_H::representation hessian_representation;

	typedef DataPackage_3<
		MatrixData<type_X>,
		MultiResponse<type_Y, 'Y'>,
		Data<sgl::matrix, 'W'> > data_type;

	FrobeniusLossWeighted() 	:
		n_variables(0),
		Y(null_matrix),
		W(null_matrix),
		lp(null_matrix)	{}

	FrobeniusLossWeighted(data_type const& data) :
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
		return static_cast<double>(2)*trans(W%(lp-Y));
	}

	void compute_hessians() const	{
		return;
	}

  const hessian_representation hessians(natural i) const {
		return hessians(i, hessian_type());
	}

	const hessian_representation hessians(natural i, hessian_identity<false> ) const {
		return 2 * W(i,0);
	}

	const hessian_representation hessians(natural i, hessian_diagonal<false> ) const {
		return 2 * trans(W.row(i));
	}

	const hessian_representation hessians(natural i, hessian_full<false> ) const {
		return 2 * arma::diagmat(W.row(i));
	}

	const hessian_representation hessians(
		natural i,
		hessian_block_diagonal<matrix, 2, false>
	) const {

		arma::field<matrix> H_field(2);

		vector d = 2*trans(W.row(i));

		H_field(0) = arma::diagmat(d.subvec(0, 1));
		H_field(1) = arma::diagmat(d.subvec(2, d.n_elem-1));

		return H_field;
	}

	const numeric sum_values() const {
		return accu(W%(lp-Y)%(lp-Y));
	}

};

// dense dense
typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, matrix,
		 	hessian_diagonal<false> > > > linear_test_diagonal_w
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, matrix,
		 	hessian_identity<false> > > > linear_test_identity
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, matrix,
		 	hessian_full<false> > > > linear_test_full
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, matrix,
		 	hessian_block_diagonal<matrix, 2, false> > > > linear_test_block_diagonal
;

// sparse dense
typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, matrix,
		 	hessian_diagonal<false> > > > linear_test_diagonal_w_spx
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, matrix,
		 	hessian_identity<false> > > > linear_test_identity_spx
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, matrix,
			hessian_full<false> > > > linear_test_full_spx
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, matrix,
			hessian_block_diagonal<matrix, 2, false> > > > linear_test_block_diagonal_spx
;


// dense sparse
typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, sparse_matrix,
			hessian_diagonal<false> > > > linear_test_diagonal_w_spy
;


typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, sparse_matrix,
			hessian_identity<false> > > > linear_test_identity_spy
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, sparse_matrix,
			hessian_full<false> > > > linear_test_full_spy
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossDense <
		FrobeniusLossWeighted < matrix, sparse_matrix,
			hessian_block_diagonal<matrix, 2, false> > > > linear_test_block_diagonal_spy
;


// sparse sparse
typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, sparse_matrix,
			hessian_diagonal<false> > > > linear_test_diagonal_w_spx_spy
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, sparse_matrix,
			hessian_identity<false> > > > linear_test_identity_spx_spy
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, sparse_matrix,
			hessian_full<false> > > > linear_test_full_spx_spy
;

typedef ObjectiveFunctionType <
	GenralizedLinearLossSparse <
		FrobeniusLossWeighted < sparse_matrix, sparse_matrix,
			hessian_block_diagonal<matrix, 2, false> > > > linear_test_block_diagonal_spx_spy
;

#endif /* TEST_OBJECTIVE_H_ */
