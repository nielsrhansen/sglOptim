/*
 Sgl template library for testing sgl objectives.
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

#ifndef SGLTESTER_H_
#define SGLTESTER_H_

class SglTester {

private:
  sgl::SglProblem const &sgl;

public:
  SglTester(const sgl::SglProblem &sgl) : sgl(sgl) {}

  template <typename T>
  sgl::natural test(T & objective, sgl::natural const max_problems) const {

    sgl::natural problem_count = 0;

    sgl::vector gradient(sgl.setup.dim);

    // Test graidnet at zero
    sgl::parameter x(sgl.setup.block_unit_dim, sgl.setup.block_dim);

    objective.at_zero();

    for (sgl::natural count = 1; count < 100; count++) {

      sgl::numeric value_at_x = objective.evaluate();

      gradient = objective.gradient();
      sgl::parameter p = convert_to_block_vector(gradient);

      // Test partial gradinets
      sgl::parameter pr(sgl.setup.block_unit_dim, sgl.setup.block_dim);
      for (sgl::natural i = 0; i < p.n_blocks; i++) {
        for (sgl::natural j = 0; j < p.block_sizes(i); j++) {

          sgl::vector bv(p.block_sizes(i), arma::fill::zeros);
          bv(j) = p.block(i)(j);

          pr.zeros();
          pr.set_block(i, bv);

          sgl::numeric t = sgl.config.stepsize_opt_penalized_initial_t;

          while (true) {

            objective.at(x - t * pr);
            sgl::numeric value = objective.evaluate();

            if (value <= value_at_x) {
              break;
            }

            t *= sgl.config.stepsize_opt_penalized_b;

            if (t < 1e-10) {

              std::ostringstream error_msg;
              error_msg
                  << "Problem with gradient detected -- testing parameter "
                  << "( " << i << ", " << j << " )";
              std::string str_msg = error_msg.str();
              report_warning(str_msg.c_str());

              problem_count++;

              if (problem_count == max_problems) {
                return problem_count;
              }

              break;
            }
          }
        }
      }

      // Test full gradient

      sgl::numeric t = sgl.config.stepsize_opt_penalized_initial_t;

			sgl::numeric value;
      while (true) {

        objective.at(x - t * p);
        value = objective.evaluate();

        if (value <= value_at_x) {
          break;
        }

        t *= sgl.config.stepsize_opt_penalized_b;

        if (t < 1e-10) {
          report_error("Problem with gradient detected -- full test");
        }
      }

      if (abs((value_at_x - value) / value) < 0.01) {
        break;
      }

      x = x - t * p;
      objective.at(x);

      // Test hessian
      for (sgl::natural i = 0; i < p.n_blocks; i++) {

        matrix H = objective.hessian_diag(i);

        if (!sgl::is_finite(H)) {
          report_error("Hessians is not finite");
        }

        if (accu(abs(symmatu(H) - H) > 1e-14) > 0) {
          report_error("Hessians are not symmetric");
        }

        if (any(eig_sym(H) < 0)) {
          report_error("Hessians are not positive definite");
        }
      }
    }

    return problem_count;
  }

private:
  sgl::parameter convert_to_block_vector(sgl::vector v) const {

    sgl::parameter p(sgl.setup.block_unit_dim, sgl.setup.block_dim);

    for (sgl::natural block_index = 0; block_index < sgl.setup.n_blocks;
         ++block_index) {
      sgl::natural block_start = sgl.setup.block_start_index(block_index);
      sgl::natural block_end = sgl.setup.block_end_index(block_index);

      p.set_block(block_index, Rcpp::wrap(v.subvec(block_start, block_end)));
    }

    return p;
  }
};

#endif /* SGLTESTER_H_ */
