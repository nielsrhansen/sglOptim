/*
 Sgl template library for optimizing sparse group lasso penalized objectives.
 Copyright (C) 2012 Martin Vincent

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

#ifndef SGLOPTIMIZER_H_
#define SGLOPTIMIZER_H_

class SglOptimizer {

private:

	sgl::SglProblem const& sgl;

public:

	const sgl::numeric alpha;

	SglOptimizer(sgl::SglProblem const& sgl, const sgl::numeric alpha);

	//Will only return the solutions (i.e x's and objective values ect) for the lambda indices values given by solution_index
	template < typename T >
	boost::tuple < sgl::block_vector_field , sgl::vector , sgl::vector >
	optimize(
		T & objective,
		sgl::vector const& lambda_sequence,
		sgl::natural_vector const& solution_index,
		bool handle_exceptions = false) const;

	template < typename T >
	sgl::natural optimize(
		sgl::parameter_field & x_field,
		sgl::natural_vector const& x_field_index,
		sgl::vector & object_value,
		sgl::vector & function_value,
		T & objective,
		sgl::vector const& lambda_sequence,
		bool handle_exceptions = false,
		bool progress_monitor = true) const;

	template < typename T >
	sgl::parameter optimize_single(
		sgl::parameter & x,
		sgl::parameter & x_old,
		sgl::vector & gradient,
		T & objective,
		sgl::numeric const& lambda) const;

private:

	template < typename T >
	void optimize_quadratic(
		T & objective,
		sgl::parameter & x,
		sgl::vector const& gradient,
		sgl::vector const& critical_bounds,
		sgl::numeric const alpha,
		sgl::numeric const lambda) const;

	void optimize_inner(
		sgl::vector const& gradient_at_x0,
		sgl::matrix const& second_order_term,
		sgl::numeric penalty_constant_L2,
		sgl::vector const& penalty_constant_L1,
		sgl::parameter_block_vector & x,
		sgl::parameter_block_vector const& x0) const;

	//Approximately Solve c + h*x + penalty*x/sqrt(x*x+r) = 0, returns the absolute value of the result
	sgl::numeric solve_main_equation(
		sgl::numeric const c,
		sgl::numeric const h,
		sgl::numeric const p,
		sgl::numeric const r,
		sgl::numeric const x) const;

	sgl::numeric update_x(
		sgl::numeric const g,
		sgl::numeric const h,
		sgl::numeric const penalty_constant_L2,
		sgl::numeric const penalty_constant_L1,
		sgl::numeric const x,
		sgl::numeric const r ) const;

	void argmin_subgradient(
		sgl::parameter_block_vector & x,
		sgl::vector const& v,
		sgl::vector const& p) const;

	sgl::numeric function_value(
		sgl::parameter_block_vector const& x,
		sgl::vector const& gradient_at_x,
		sgl::matrix const& second_order_term,
		sgl::numeric penalty_constant_L2,
		sgl::vector const& penalty_constant_L1) const;

	void locate_safe_point(
		sgl::parameter_block_vector & safe_point,
		sgl::parameter_block_vector const& x,
		sgl::vector const& gradient_at_x,
		sgl::matrix const& second_order_term,
		sgl::numeric penalty_constant_L2,
		sgl::vector const& penalty_constant_L1) const;

	template < typename T >
	sgl::numeric stepsize_optimize_penalized(
		T & objective,
		sgl::parameter const& x1,
		sgl::parameter const& x0,
		sgl::vector const& gradient_at_x0,
		sgl::numeric const likelihood_at_x0,
		sgl::numeric const lambda) const;

	bool is_stopping_criteria_fulfilled(
		sgl::parameter const& x,
		sgl::parameter const& x_old,
		sgl::numeric const f,
		sgl::numeric const f_old) const;

	void interrupt_check() const;

	void init_interrupt_handler();
};

inline SglOptimizer::SglOptimizer(const sgl::SglProblem & sgl, const sgl::numeric alpha)
		: sgl(sgl), alpha(alpha)
{
	if (0 > alpha || alpha > 1)
	{
		throw std::domain_error("alpha is not in the range 0 to 1");
	}
}

template < typename T >
inline boost::tuple < sgl::block_vector_field , sgl::vector , sgl::vector >
	SglOptimizer::optimize(
		T & objective,
		sgl::vector const& lambda_sequence,
		sgl::natural_vector const& solution_index,
		bool handle_exceptions) const {

	sgl::block_vector_field x_field(solution_index.n_elem);
	sgl::vector object_value(solution_index.n_elem);
	sgl::vector function_value(solution_index.n_elem);

	optimize(
		x_field,
		solution_index,
		object_value,
		function_value,
		objective,
		lambda_sequence,
		handle_exceptions
	);

	return boost::make_tuple(x_field, object_value, function_value);
}

bool SglOptimizer::is_stopping_criteria_fulfilled(
	sgl::parameter const& x,
	sgl::parameter const& x_old,
	sgl::numeric const f,
	sgl::numeric const f_old) const
{

	//Check stopping criteria

	//Layer : Objective function value stopping rule

	if (abs(f_old - f) / abs(f_old) > sgl.config.tolerance_penalized_outer_loop_gamma)
	{
		return false;
	}

	//Layer : Distance stopping rule

	sgl::numeric d = sgl::dist(x_old, x);

	if (d > sgl.config.tolerance_penalized_outer_loop_alpha)
	{
		return false;
	}

	//Layer : Model stopping rule

	if (sgl::discrete_dist(x_old, x) > sgl.config.tolerance_penalized_outer_loop_beta)
	{
		return false;
	}

	return true;

}

//Given x and gradient should correspond when called
// x and gradient will not correspond after the call (for the k'th x, gradient will be the k-1'th)
//Returns the k-1'th parameter - corresponding to last updated gradient
//x and x_old must have same size
template < typename T >
sgl::parameter SglOptimizer::optimize_single(
  sgl::parameter & x,
  sgl::parameter & x_old,
  sgl::vector & gradient,
  T & objective,
  sgl::numeric const& lambda) const {

  TIMER_START;
  DEBUG_ENTER

  //Quadratic approximation loop

  sgl::numeric f;
  sgl::numeric f_old;

  //Initialise converges checker
  CONVERGENCE_CHECK(sgl.config.max_iterations_outer);

  sgl::vector critical_bounds(sgl.setup.n_blocks); //Note this will only be used when use_bound_optimization = true

  do {

    CONVERGENCE_CHECK_INCREASE;

    x_old = x;
    f_old = objective.evaluate() + sgl.penalty(x, alpha, lambda);

    //Compute critical bounds
    if (sgl.config.use_bound_optimization) {
      critical_bounds = sgl.compute_bounds(gradient, x, alpha, lambda);
    }

    optimize_quadratic(objective, x, gradient, critical_bounds, alpha, lambda);

    objective.at(x);
    f = objective.evaluate() + sgl.penalty(x, alpha, lambda);

    // Stepsize optimization
		if ( sgl.config.use_stepsize_optimization_in_penalizeed_loop &&
      f > f_old &&
      ! is_stopping_criteria_fulfilled(x, x_old, f, f_old) ) {

      TIMER_START

      objective.at(x_old);

      // Find stepsize
      sgl::numeric t = stepsize_optimize_penalized(
        objective,
        x,
        x_old,
        gradient,
        objective.evaluate(),
        lambda
      );

      if (t != 1)	{
        x = (1 - t) * x_old + t * x;
      }

      #ifdef SGL_DEBUG_INFO_STEPSIZE
        Rcpp::Rcout << "stepsize = " << t << std::endl;
      #endif

      objective.at(x);
      f = objective.evaluate() + sgl.penalty(x, alpha, lambda);

    }

    #ifdef SGL_DEBUG_INFO_QUADRATIC
      Rcpp::Rcout << " rel func. improvement = "<< (f_old - f) / abs(f_old) << std::endl;
      Rcpp::Rcout << " parameter distance = " << sgl::dist(x_old, x) << std::endl;
      Rcpp::Rcout << " parameter discrete distance = " << sgl::discrete_dist(x_old, x) << std::endl;
      Rcpp::Rcout << " current penalized objective value = " << f << std::endl;
      Rcpp::Rcout << " old penalized objective value = " << f_old << std::endl;
      Rcpp::Rcout << " current objective value = " <<  objective.evaluate() << std::endl;
    #endif

    if ( ! is_stopping_criteria_fulfilled(x, x_old, f, f_old) ) {

      //Continue quadratic loop

      ASSERT_IS_NON_NEGATIVE(f_old - f + 1e-10); //NOTE configable threshold
      gradient = objective.gradient();

    }

    else 	{

      //Exit quadratic loop

      return x_old;
    }

	} while (true);

}

template < typename T >
sgl::natural SglOptimizer::optimize(
		sgl::parameter_field & x_field,
		sgl::natural_vector const& needed_solutions,
		sgl::vector & object_value,
		sgl::vector & function_value,
		T & objective,
		sgl::vector const& lambda_sequence,
		bool handle_exceptions,
		bool progress_monitor) const {

	//Start scope timer, note will only be activated if DO_TIMING is defined
	TIMER_START;
	DEBUG_ENTER

	//Ensure x_field_index is ordered
	sgl::natural_vector x_field_order = sort(needed_solutions,  "ascend");

	sgl::vector gradient(sgl.setup.dim);
	sgl::parameter x(sgl.setup.block_unit_dim, sgl.setup.block_dim);
	sgl::parameter x0(sgl.setup.block_unit_dim, sgl.setup.block_dim);

	//Start at zero
	x.zeros();
	x0.zeros();
	objective.at_zero();
	gradient = objective.gradient();

	//Ensure gradient is finite
	ASSERT_IS_FINITE(gradient);

	//Lambda loop
	sgl::natural lambda_index = 0;

	//Needed solutions index
	sgl::natural x_field_index = 0;

	try
	{

		// create progress monitor
		Progress p(lambda_sequence.n_elem, sgl.config.verbose & progress_monitor);

		while (!p.is_aborted())
		{

			sgl::numeric const lambda = lambda_sequence(lambda_index);

			optimize_single(x, x0, gradient, objective, lambda);

			//Check if we must save the solution
			if (lambda_index == x_field_order(x_field_index))
			{

				//Save solution
				x_field(x_field_index) = x;

				//Save objective function values
				object_value(x_field_index) = objective.evaluate();

				function_value(x_field_index) =
					object_value(x_field_index) + sgl.penalty(x, alpha, lambda);

				//Next x field
				++x_field_index;
			}

			//next lambda
			++lambda_index;

			//Increas progress monitor
			p.increment();

			if (	lambda_index >= lambda_sequence.n_elem ||
						x_field_index >= x_field_order.n_elem)
			{
				//No more lambda values or no more solutions needed - exit
				break;
			}

			//Go one step back, (avoid computing the gradient) - hence start at x0
			x = x0;
			objective.at(x0);
		}

	} catch (SGL_EXCEPTIONS & e)
	{

		//Reset intrrupt flag
		SGL_INTERRUPT_RESET;

		throw;

	} catch (SGL_EXCEPTIONS_GENRAL)
	{

		//Reset intrrupt flag
		SGL_INTERRUPT_RESET;

		throw;
	}

	return x_field_index;
}

template < typename T >
sgl::numeric SglOptimizer::stepsize_optimize_penalized(
	T & objective,
	sgl::parameter const& x1,
	sgl::parameter const& x0,
	sgl::vector const& gradient_at_x0,
	sgl::numeric const objective_at_x0,
	sgl::numeric const lambda) const
{

	TIMER_START;

	sgl::numeric t = sgl.config.stepsize_opt_penalized_initial_t;

	sgl::numeric penalty_at_x0 = sgl.penalty(x0, alpha, lambda);

	sgl::numeric delta = sgl.config.stepsize_opt_penalized_a
			* (dot(gradient_at_x0, (x1 - x0)) + sgl.penalty(x1, alpha, lambda) - penalty_at_x0);

	sgl::numeric value_at_x0 = objective_at_x0 + penalty_at_x0;

	sgl::numeric value;

	while (true) {

		objective.at((1 - t) * x0 + t * x1);
		value = objective.evaluate() + sgl.penalty((1 - t) * x0 + t * x1, alpha, lambda);

		if(value - (value_at_x0 + t * delta) < 0) {
			break;
		}

		if (t - std::numeric_limits< sgl::numeric >::epsilon() < 0)
		{
			// We could not find a descent direction -- try again
			// Do not return 0, as this will result in the algorithm returning an non optimal x
			return 0.5;
		}

		t *= sgl.config.stepsize_opt_penalized_b;

	}

	return t;
}

//gradient should be the gradient at x
template < typename T >
inline void SglOptimizer::optimize_quadratic(
	T & objective,
	sgl::parameter & x,
	sgl::vector const& gradient,
	sgl::vector const& critical_bounds,
	sgl::numeric const alpha,
	sgl::numeric const lambda) const {

	TIMER_START
	DEBUG_ENTER

	//Initialize converges checker
	CONVERGENCE_CHECK(1e4); //NOTE configable converges checker

	sgl::vector block_gradient;
	sgl::parameter_block_vector x_new;

	sgl::numeric dist;

	do
	{

		CONVERGENCE_CHECK_INCREASE;

	//FIXME	Progress::check_abort();

		#ifdef SGL_DEBUG_INFO_GB_OPT
			sgl::natural computed_gbs = 0;
		#endif

		// **** Regular coordinate decent loop

		// Reset distance
		dist = 0;

		for (sgl::natural block_index = 0; block_index < sgl.setup.n_blocks; ++block_index) 	{

			block_gradient.set_size(sgl.setup.block_dim(block_index));
			x_new.set_size(sgl.setup.block_dim(block_index));

			// **** Block optimization

			sgl::natural block_start = sgl.setup.block_start_index(block_index);
			sgl::natural block_end = sgl.setup.block_end_index(block_index);

			if ( ! sgl.config.use_bound_optimization
					|| (!x.is_block_zero(block_index)
					|| (critical_bounds(block_index) <= objective.hessian_bound_level0()
					&& critical_bounds(block_index)	<= objective.hessian_bound_level1(block_index)))) {

				//Block could be active, check needed

				#ifdef SGL_DEBUG_INFO_GB_OPT
					++computed_gbs;
				#endif

				block_gradient = gradient.subvec(block_start, block_end)
						+ objective.compute_block_gradient(block_index);

				//Check if block is active

				bool block_is_active;

				if (x.is_block_zero(block_index))	{

					block_is_active =
						sgl.is_block_active(block_gradient, block_index, alpha, lambda);

				}

				else {

					block_is_active =
						sgl.is_block_active(
							block_gradient - objective.hessian_diag(block_index) * x.block(block_index),
							block_index, alpha, lambda);

				}

				if (block_is_active) {

					sgl::parameter_block_vector const x_block(x.block(block_index));

					//Block active (non zero), Optimise

					optimize_inner(
						block_gradient,
						objective.hessian_diag(block_index),
						lambda * (1 - alpha) * sgl.setup.L2_penalty_weight(block_index),
						lambda * alpha * sgl.setup.L1_penalty_weight(block_index),
						x_new,
						x.block(block_index));

					// Update

					double const dist_block = sgl::max_dist(x_block, x_new);

					// Max dist
					if (dist < dist_block) {
						dist = dist_block;
					}

					objective.hessian_update(block_index, x_new);

					x.set_block(block_index, x_new);

					continue;

				}

			}

			//Block is inactive

			#ifdef SGL_DEBUG_BLOCK_ACTIVE
				// This is a complex debug section,
				// if compiled with this section active performance will be considerably lower than optimal performance

				block_gradient = gradient.subvec(block_start, block_end) + objective.compute_block_gradient(block_index);

				if (!x.is_block_zero(block_index)) 	{
					block_gradient -= objective.hessian_diag(block_index) * x.block(block_index);
				}

				//Check if block is active
				if (sgl.is_block_active(block_gradient, block_index, alpha, lambda)) {

					//NOTE update this line with new function names
					//Rcpp::Rcout << "block = " << block_index << " gab = " << sgl.compute_K(abs(block_gradient) - lambda * alpha * sgl.setup.L1_penalty_weight(block_index), 0) - sgl::square(lambda * (1 - alpha) * sgl.setup.L2_penalty_weight(block_index)) << endl;

					Rcpp::Rcout << "critical bound = " << critical_bounds(block_index) << " hessian level 0 bound = " << objective.hessian_bound_level0()
					<< " hessian level 1 bound = " << objective.hessian_bound_level1(block_index) << std::endl;

					throw std::runtime_error("error - hessian bound");
				}
			#endif

			if (x.is_block_zero(block_index))	{
				continue;
			}

			// Update
			x_new.zeros();

			sgl::parameter_block_vector x_block(x.block(block_index));
			double const dist_block = sgl::max_dist(x_block, x_new);

			// Max dist
			if (dist < dist_block) {
				dist = dist_block;
			}

			objective.hessian_update(block_index, x_new);
			x.set_block_zero(block_index);
		}

		#ifdef SGL_DEBUG_INFO_GB_OPT
			Rcpp::Rcout << "Computed block gradients " << computed_gbs << std::endl;
		#endif

	} while (dist > sgl.config.tolerance_penalized_middel_loop_alpha);

}

void SglOptimizer::optimize_inner(
	sgl::vector const& gradient_at_x0,
	sgl::matrix const& second_order_term,
	sgl::numeric penalty_constant_L2,
	sgl::vector const& penalty_constant_L1,
	sgl::parameter_block_vector & x,
	sgl::parameter_block_vector const& x0) const {

	TIMER_START
	DEBUG_ENTER

	//Initialize converges checker
	CONVERGENCE_CHECK(sgl.config.inner_loop_convergence_limit);

	const sgl::natural dim = x0.n_elem;

	sgl::vector gradient = gradient_at_x0;
	x = x0; //start at x0

  sgl::numeric sumsq = arma::as_scalar(sum(square(x)));
	sgl::parameter_block_vector x_old(dim);

	do {

		CONVERGENCE_CHECK_INCREASE;

		SGL_INTERRUPT_CHECK;

		x_old = x;

		for (sgl::natural i = 0; i < dim; i++) {

			sgl::numeric const xi = x(i);

			//Compute new x
			sgl::numeric x_new = update_x(
				gradient(i),
				second_order_term(i, i),
				penalty_constant_L2,
				penalty_constant_L1(i),
				xi,
				sgl::pos(sumsq - sgl::square(xi))
			);

			ASSERT_IS_NUMBER(x_new);
			ASSERT_IS_FINITE(x_new);

			//Update gradient and x
			if (abs(x_new - xi)/abs(xi) > 1e-8) { //NOTE configable

				gradient += (x_new - xi) * second_order_term.col(i);
				sumsq += sgl::square(x_new) - sgl::square(xi);

				x(i) = x_new;
			}

		}

		//Check if we ended up near zero
		if (sumsq < 1e-20) {

			sgl::numeric f_value = function_value(
				x,
				gradient,
				second_order_term,
				penalty_constant_L2,
				penalty_constant_L1
			);

			if(f_value >= 0) {

				//Find new x
				locate_safe_point(
					x,
					x0,
					gradient_at_x0,
					second_order_term,
					penalty_constant_L2,
					penalty_constant_L1
				);

				//update gradient
				gradient = gradient_at_x0 + second_order_term * (x - x0);
			}
		}

	} while (sgl::max_dist(x_old, x) > sgl.config.tolerance_penalized_inner_loop_alpha
			|| sgl::discrete_dist(x_old, x) > sgl.config.tolerance_penalized_inner_loop_beta);

	ASSERT_IS_NON_ZERO(x);
}

//NOTE better name for function
void SglOptimizer::locate_safe_point(
	sgl::parameter_block_vector & safe_point,
	sgl::parameter_block_vector const& x,
	sgl::vector const& gradient_at_x,
	sgl::matrix const& second_order_term,
	sgl::numeric penalty_constant_L2,
	sgl::vector const& penalty_constant_L1) const {

	sgl::parameter_block_vector x_decent(safe_point.n_elem);
	argmin_subgradient(x_decent, gradient_at_x - second_order_term * x, penalty_constant_L1);

	safe_point = x_decent;

	sgl::numeric t = 1; //NOTE configable

	while (true) {

	sgl::numeric value = function_value(
		safe_point,
		gradient_at_x + second_order_term * (safe_point - x),
		second_order_term,
		penalty_constant_L2,
		penalty_constant_L1
	);

		if(value < 0) {
			break;
		}

		t = 0.9 * t;

		safe_point = t * x_decent;
	}
}

sgl::numeric SglOptimizer::function_value(
	sgl::parameter_block_vector const& x,
	sgl::vector const& gradient_at_x,
	sgl::matrix const& second_order_term,
	sgl::numeric penalty_constant_L2,
	sgl::vector const& penalty_constant_L1) const {

	return arma::as_scalar(
		trans(gradient_at_x - 1/2 * second_order_term * x) * x
		+ penalty_constant_L2 * sqrt(sum(square(x)))
		+ sum(penalty_constant_L1 % abs(x))
	);
}

void SglOptimizer::argmin_subgradient(sgl::parameter_block_vector & x, sgl::vector const& v,
		sgl::vector const& p) const
{

	for (sgl::natural i = 0; i < v.n_elem; ++i)
	{
		if (abs(v(i)) > p(i))
		{
			x(i) = p(i) * sgl::sign(v(i)) - v(i);
		}
		else
		{
			x(i) = 0;
		}
	}
}

//r = sum_{j != i} x_j^2
sgl::numeric SglOptimizer::update_x(
	sgl::numeric g,
	sgl::numeric const h,
	sgl::numeric const penalty_constant_L2,
	sgl::numeric const penalty_constant_L1,
	sgl::numeric const x,
	sgl::numeric const r) const
{

	if (h == 0)
	{
		return 0;
	}

	//Special case no penalty
	if (penalty_constant_L1 == 0 && penalty_constant_L2 == 0)
	{
		return x - g / h;
	}

	//Special case L2 penalty = 0
	if (penalty_constant_L2 == 0)
	{

		sgl::numeric const penalty = penalty_constant_L1;

		if (abs(g - h * x) <= penalty)
		{
			return 0;
		}

		if (g - h * x < -penalty)
		{
			return x - (g + penalty) / h;
		}

		return x + (penalty - g) / h;
	}

	//Case r = 0
	if (r == 0)
	{

		sgl::numeric const penalty = penalty_constant_L1 + penalty_constant_L2;

		if (abs(g - h * x) <= penalty)
		{
			return 0;
		}

		if (g - h * x < -penalty)
		{
			return x - (penalty + g) / h;
		}

		return x + (penalty - g) / h;
	}

	//Case r non zero

	//Special case L1 penalty = 0
	if (penalty_constant_L1 == 0)
	{

		sgl::numeric const penalty = penalty_constant_L2;

		return -sgl::sign(g - h * x) * solve_main_equation(abs(g - h * x), h, penalty, r, x);
	}

	//L1 penalty and L2 penalty non zero
	sgl::numeric const penalty = penalty_constant_L1;

	if (abs(g - h * x) <= penalty)
	{
		return 0;
	}

	if (g - h * x < -penalty)
	{

		//x pos
		return solve_main_equation(abs(g - h * x + penalty_constant_L1), h, penalty_constant_L2, r, x);
	}

	//x negative
	return -solve_main_equation(abs(g - h * x - penalty_constant_L1), h, penalty_constant_L2, r, x);

}

// solver for c + h*x + p*x/sqrt(x^2+r) = 0
sgl::numeric SglOptimizer::solve_main_equation(
		sgl::numeric const c,
		sgl::numeric const h,
		sgl::numeric const p,
		sgl::numeric const r,
		sgl::numeric const x_initial) const
{

	//Start scope timer, note will only be activated if DO_TIMING is defined
	//TIMER_START;

	//Initialise converges checker
	CONVERGENCE_CHECK(1e8); //NOTE configable

	//Domain checks
	ASSERT_IS_POSITIVE(c);
	ASSERT_IS_POSITIVE(h);
	ASSERT_IS_POSITIVE(p);
	ASSERT_IS_POSITIVE(r);

	sgl::numeric x0 = 0;
	sgl::numeric x1 = -c / h;

	//use initial point

	sgl::numeric const x_init = -abs(x_initial);

	if (x_init > x1)
	{
		if (c + h * x_init + p * x_init / sqrt(x_init * x_init + r) > 0)
		{
			x0 = x_init;
		}
		else
		{
			x1 = x_init;
		}
	}

	do
	{
		CONVERGENCE_CHECK_INCREASE;

		sgl::numeric new_x = x1 + (x0 - x1) / 2;

		sgl::numeric value = c + h * new_x + p * new_x / sqrt(new_x * new_x + r);

		if (abs(value) < 1e-10) //NOTE configurable
		{
			x0 = new_x;
			break;
		}
		else if (value > 0)
		{
			x0 = new_x;
		}
		else
		{
			x1 = new_x;
		}

	} while (abs(x0 - x1) > sgl.config.tolerance_penalized_main_equation_loop);

	ASSERT_IS_FINITE(x0);

	return abs(x0);
}

#endif /* SGLOPTIMIZER_H_ */
