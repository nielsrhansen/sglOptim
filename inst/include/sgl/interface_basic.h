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

#ifndef INTERFACE_BASIC_H_
#define INTERFACE_BASIC_H_

template<typename ObjectiveFunctionType>
class Interface {

public:

	const sgl::numeric alpha;

	sgl::SglProblem const sgl;
	sgl::SglOptimizer const optimizer;
	ObjectiveFunctionType const& objective_type;

	/**
	 * Construct a Interface.
	 *
	 * @param objective_type objective function type
	 * @param dim_config dimension configuration for the sgl optimizer
	 * @param alpha alpha values of the sgl optimizer
	 * @param config algorithm configuration
	 */
	Interface(
		ObjectiveFunctionType const& objective_type,
		sgl::DimConfig const& dim_config,
		sgl::numeric alpha,
		AlgorithmConfiguration const& config) :
			alpha(alpha),
			sgl(dim_config, config),
			optimizer(sgl, alpha),
			objective_type(objective_type) {}

	/**
	 * Optimize the given objective.
	 *
	 * @param x_field will after the call returns contain the fitted parameters
	 * @param needed_solutions indices of the lambda values for the needed solutions
	 * @param object_value will after the call returns contain the value of the objective at the give lambda values
	 * @param function_value will after the call returns contain the value of the penalty + objective at the give lambda values
	 * @param lambda_sequence the lambda sequence to optimize over
	 */
	natural optimize(
		parameter_field & x_field,
		natural_vector needed_solutions,
		vector & object_value,
		vector & function_value,
		vector const& lambda_sequence) const;

 //subsampling
	template<typename Predictor>

		boost::tuple<
			arma::field< arma::field < typename Predictor::response_type > >,
			natural_vector,
			natural_vector>

		subsamplerun(
			Predictor const& predictor,
			vector const& lambda_sequence,
    	typename ObjectiveFunctionType::data_type const& test_data) const;

	//Lambda

	/**
	 *
	 * @return
	 */
	numeric lambda_max() const;

	/**
	 *
	 * @param lambda_max
	 * @param lambda_min
	 * @param n
	 * @return
	 */
	vector	lambda_sequence(
		numeric lambda_max,
		numeric lambda_min,
		natural n) const;
};

template<typename ObjectiveFunctionType>
sgl::numeric Interface<ObjectiveFunctionType>::lambda_max() const {

	typename ObjectiveFunctionType::instance_type objective = objective_type.create_instance(sgl.setup);

	objective.at_zero();

	if(sgl.has_unpenalized_paramters(alpha)) {

		//Fit non penalized parameters
		sgl::parameter_field x_field(1);
		sgl::natural_vector needed_solutions(1, arma::fill::zeros);
		sgl::vector object_value(1);
		sgl::vector function_value(1);
		sgl::vector lambda_sequence(1);

		lambda_sequence(0) =  1e100; //NOTE configable

		//Optimize non penalized paramters
		optimizer.optimize(
			x_field,
			needed_solutions,
			object_value,
			function_value,
			objective,
			lambda_sequence, true, false);

	}

	//Compute and return lambda max
	return sgl.compute_critical_lambda(objective.gradient(), alpha);
}

template<typename ObjectiveFunctionType>
sgl::vector Interface<ObjectiveFunctionType>::lambda_sequence(
		sgl::numeric lambda_max,
		sgl::numeric lambda_min,
		sgl::natural n) const {

	sgl::vector lambda_sequence(n);

	lambda_sequence(n - 1) = lambda_min;

	sgl::numeric const a = exp((log(lambda_max) - log(lambda_min)) / (n - 1));

	for (sgl::natural i = 1; i < n; i++) {
		lambda_sequence(n - 1 - i) = a * lambda_sequence(n - i);
	}

	return lambda_sequence;
}

template<typename ObjectiveFunctionType>
inline sgl::natural Interface<ObjectiveFunctionType>::optimize(
		sgl::parameter_field & x_field,
		sgl::natural_vector needed_solutions,
		sgl::vector & object_value,
		sgl::vector & function_value,
		sgl::vector const& lambda_sequence) const {

	//Domain checks
	if ( ! sgl::is_decreasing(lambda_sequence)
			|| ! sgl::is_positive(lambda_sequence)) {

		throw std::domain_error("the lambda sequence must be decreasing and positive");
	}

	//NOTE check that all elements of needed_solutions are unique and less than the length of lambda_sequence

	typename ObjectiveFunctionType::instance_type objective = objective_type.create_instance(sgl.setup);

	sgl::natural n_solutions = optimizer.optimize(
		x_field,
		needed_solutions,
		object_value,
		function_value,
		objective,
		lambda_sequence, true);

		return n_solutions;
}

template<typename ObjectiveFunctionType>
template<typename Predictor>
inline boost::tuple<
				arma::field< arma::field < typename Predictor::response_type > >,
				sgl::natural_vector,
				sgl::natural_vector>

					Interface<ObjectiveFunctionType>::subsamplerun(
						Predictor const& predictor,
						sgl::vector const& lambda_sequence,
						typename ObjectiveFunctionType::data_type const& test_data) const {

	//Domain checks
	if ( ! sgl::is_decreasing(lambda_sequence)
			|| ! sgl::is_positive(lambda_sequence) ) {

			throw std::domain_error("subsamplerun : the lambda sequence must be decreasing and positive");
	}

	//length of lambda sequence
	sgl::index len_lambda = lambda_sequence.n_elem;
	sgl::natural n_samples = test_data.data_matrix.n_rows;

	//result containers
	arma::field < arma::field < typename Predictor::response_type > > responses(n_samples);

		// Set size of inner field
		for (natural j = 0; j < n_samples; ++j) {
			responses(j).set_size(len_lambda);
		}

	sgl::natural_vector number_of_features(len_lambda);
	sgl::natural_vector number_of_parameters(len_lambda);

  // Create objective
 	typename ObjectiveFunctionType::instance_type
		objective(objective_type.create_instance(sgl.setup));

	//Fit
	sgl::parameter x(sgl.setup.block_unit_dim, sgl.setup.block_dim);
	sgl::parameter x0(sgl.setup.block_unit_dim, sgl.setup.block_dim);
	sgl::vector gradient(sgl.setup.dim);

	//Start at zero
	x.zeros();
	x0.zeros();
	objective.at_zero();
	gradient = objective.gradient();

	//Lambda loop
	sgl::natural lambda_index = 0;

	while (true) {

		// Run single opt
		sgl::numeric const lambda = lambda_sequence(lambda_index);
	 	optimizer.optimize_single(x, x0, gradient, objective,	lambda);

		//set number of features / parameters
		number_of_features(lambda_index) = x.n_nonzero_blocks;
		number_of_parameters(lambda_index) = x.n_nonzero;

		//Predict
		predictor.predict(responses, test_data, x, lambda_index);

		//next lambda
		++lambda_index;

	 	if (lambda_index >= len_lambda) {
			//No more lambda values - exit
			break;
		}

		//Go one step back, (avoid computing the gradient) - hence start at x0
		x = x0;
		objective.at(x0);
	}

	return boost::make_tuple(
		responses,
		number_of_features,
		number_of_parameters);
}

#endif /* INTERFACE_BASIC_H_ */
