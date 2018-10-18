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

#ifndef LINEAR_PREDICTOR_H_
#define LINEAR_PREDICTOR_H_



template < typename T , typename R >
class LinearPredictor {

public:

	typedef MatrixData < T > data_type;
	typedef R response_type;

	// Returns a filed of fileds of response types
	// structered in the following way:
	// sample 1 - model (lambda) 1 - response element
	//          - model (lambda) 2 - response element
	//          - ...
	// sample 2 - model (lambda) 1 - response element
	//          - model (lambda) 2 - response element
	//          - ...
	// ...
	const arma::field < arma::field < response_type > > predict(
		data_type const& data,
		sparse_matrix_field const& parameters) const {

			natural n_samples = data.data_matrix.n_rows;
			natural n_models = parameters.n_elem;

			arma::field < arma::field < response_type > > responses(n_samples);

			// Set size of inner field
			for (natural j = 0; j < n_samples; ++j) {
				responses(j).set_size(n_models);
			}

			// do predictions
			for (natural i = 0; i < n_models; ++ i) {
				predict(responses, data, parameters(i), i);
			}

		return responses;
	}

	// insert predictions in responses at lambda index = index.
	void predict(
		arma::field < arma::field < response_type > > & responses,
		data_type const& data,
		sparse_matrix const& x,
		natural index) const {

			natural n_samples = data.data_matrix.n_rows;

			matrix lp(data.data_matrix * trans(x));
			lp = trans(lp);

			for (natural i = 0; i < n_samples; ++i) {
				responses(i)(index) = response_type(static_cast < vector >(lp.col(i)));
			}

	}

};

#endif /* LINEAR_PREDICTOR_H_ */
