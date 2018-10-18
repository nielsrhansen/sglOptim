
#     Description of this R script:
#     R interface rotine for testing a sgl objective
#
#     Intended for use with R.
#     Copyright (C) 2014 Martin Vincent
#
#     This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <http://www.gnu.org/licenses/>
#

#' @title Test a sgl-Objective
#' @description
#' This function will run tests on the gradient and hessian functions implemented in a C++ objective module.
#' Detected problems will be printed to the console.'
#'
#' @param module_name reference to objective specific C++ routines.
#' @param PACKAGE name of the calling package.
#' @param data a list of data objects -- will be parsed to the specified module.
#' @param parameterGrouping grouping of parameters, a vector of length \eqn{p}. Each element of the vector specifying the group of the parameters in the corresponding column of \eqn{\beta}.
#' @param groupWeights the group weights, a vector of length \code{length(unique(parameterGrouping))} (the number of groups).
#' @param parameterWeights a matrix of size \eqn{q \times p}.
#' @param algorithm.config the algorithm configuration to be used.
#' @return The number of found problems
#' @author Martin Vincent
#' @export
#' @useDynLib sglOptim, .registration=TRUE
#' @importFrom utils packageVersion
#' @import Matrix
sgl_test <- function(
  module_name,
  PACKAGE,
  data,
  parameterGrouping,
  groupWeights,
  parameterWeights,
  algorithm.config = sgl.standard.config) {

  alpha <- 1

	args <- prepare.args(
    data = data,
    parameterGrouping = parameterGrouping,
    groupWeights = groupWeights,
    parameterWeights = parameterWeights,
    alpha =  alpha
  )

	call_sym <- paste(module_name, "sgl_test", sep="_")
  problems <- .Call(call_sym, PACKAGE = PACKAGE,
    args$data,
    args$block_dim,
    args$groupWeights,
    args$parameterWeights,
    algorithm.config
  )

  return(problems)

}
