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

#ifndef MSGL_ALGORITHM_CONFIG_H_
#define MSGL_ALGORITHM_CONFIG_H_

template<typename type>
static type getConfigAttribute(rList const& config, std::string const& name) {

	int index = config.getIndex(name);
	if (index >= 0) {

		return get_value < type > (config.get(index));

	} else {

    std::string msg = "Missing configuration parameter : ";
		throw std::domain_error(msg.append(name).c_str());
		return type(); //avoid compiler warnings
	}
}

class AlgorithmConfiguration {

public:

	sgl::numeric const tolerance_penalized_main_equation_loop;

	sgl::numeric const tolerance_penalized_inner_loop_alpha;
	sgl::numeric const tolerance_penalized_inner_loop_beta;

	sgl::numeric const tolerance_penalized_middel_loop_alpha;

	sgl::numeric const tolerance_penalized_outer_loop_alpha;
	sgl::numeric const tolerance_penalized_outer_loop_beta;
	sgl::numeric const tolerance_penalized_outer_loop_gamma;

	bool const use_bound_optimization;

	bool const use_stepsize_optimization_in_penalizeed_loop;
	sgl::numeric const stepsize_opt_penalized_initial_t;
	sgl::numeric const stepsize_opt_penalized_a;
	sgl::numeric const stepsize_opt_penalized_b;

	sgl::natural const max_iterations_outer;
	sgl::natural const inner_loop_convergence_limit;

	bool const verbose;

	AlgorithmConfiguration(rList const& config) :

			tolerance_penalized_main_equation_loop(
					getConfigAttribute<sgl::numeric>(config,
							"tolerance_penalized_main_equation_loop")),

			tolerance_penalized_inner_loop_alpha(
					getConfigAttribute<sgl::numeric>(config,
							"tolerance_penalized_inner_loop_alpha")),

			tolerance_penalized_inner_loop_beta(
					getConfigAttribute<sgl::numeric>(config,
							"tolerance_penalized_inner_loop_beta")),

			tolerance_penalized_middel_loop_alpha(
					getConfigAttribute<sgl::numeric>(config,
							"tolerance_penalized_middel_loop_alpha")),

			tolerance_penalized_outer_loop_alpha(
					getConfigAttribute<sgl::numeric>(config,
							"tolerance_penalized_outer_loop_alpha")),

			tolerance_penalized_outer_loop_beta(
					getConfigAttribute<sgl::numeric>(config,
							"tolerance_penalized_outer_loop_beta")),

			tolerance_penalized_outer_loop_gamma(
					getConfigAttribute<sgl::numeric>(config,
							"tolerance_penalized_outer_loop_gamma")),

			use_bound_optimization(
					getConfigAttribute<bool>(config,
							"use_bound_optimization")),

			use_stepsize_optimization_in_penalizeed_loop(
					getConfigAttribute<bool>(config,
							"use_stepsize_optimization_in_penalizeed_loop")),

			stepsize_opt_penalized_initial_t(
					getConfigAttribute<sgl::numeric>(config,
							"stepsize_opt_penalized_initial_t")),

			stepsize_opt_penalized_a(
					getConfigAttribute<sgl::numeric>(config,
							"stepsize_opt_penalized_a")),

			stepsize_opt_penalized_b(
					getConfigAttribute<sgl::numeric>(config,
							"stepsize_opt_penalized_b")),

			max_iterations_outer(
					getConfigAttribute<sgl::natural>(config,
							"max_iterations_outer")),

			inner_loop_convergence_limit(
					getConfigAttribute<sgl::natural>(config,
							"inner_loop_convergence_limit")),

			verbose(getConfigAttribute<bool>(config, "verbose")) {
	}

	AlgorithmConfiguration() :
		tolerance_penalized_main_equation_loop(1e-10),

		tolerance_penalized_inner_loop_alpha(1e-4),
		tolerance_penalized_inner_loop_beta(0),

		tolerance_penalized_middel_loop_alpha(0.01),

		tolerance_penalized_outer_loop_alpha(0.01),
		tolerance_penalized_outer_loop_beta(0),
		tolerance_penalized_outer_loop_gamma(5e-4),

		use_bound_optimization(true),

		use_stepsize_optimization_in_penalizeed_loop(true),
		stepsize_opt_penalized_initial_t(1),
		stepsize_opt_penalized_a(0.1),
		stepsize_opt_penalized_b(0.5),

		max_iterations_outer(1e5),
		inner_loop_convergence_limit(1e5),

		verbose(true) {
	}
};

#endif /* MSGL_ALGORITHM_CONFIG_H_ */
