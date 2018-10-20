#
#     Description of this R script:
#     R interface to routine for computing a lambda sequence for the regularization path
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


#' @title Computing a Lambda Sequence
#'
#' @description
#' Computes a decreasing lambda sequence of length \code{d}.
#' The sequence ranges from a data determined maximal lambda \eqn{\lambda_\textrm{max}} to the user supplied \code{lambda.min}.
#'
#' @param module_name reference to objective specific C++ routines.
#' @param PACKAGE name of the calling package.
#' @param data list of data objects -- will be parsed to the specified module.
#' @param parameterGrouping grouping of parameters, a vector of length \eqn{p}. Each element of the vector specifying the group of the parameters in the corresponding column of \eqn{\beta}.
#' @param groupWeights group weights, a vector of length \code{length(unique(parameterGrouping))} (the number of groups).
#' @param parameterWeights parameters weights, a matrix of size \eqn{q \times p}.
#' @param alpha the \eqn{\alpha} value, 0 for group lasso, 1 for lasso, between 0 and 1 gives a sparse group lasso penalty.
#' @param d the length of the lambda sequence.
#' @param lambda.min the smallest lambda value in the computed sequence.
#' @param lambda.min.rel is lambda.min relative to lambda.max ? (i.e. actual lambda min used is \code{lambda.min*lambda.max}, with \code{lambda.max} the computed maximal lambda value)
#' @param algorithm.config the algorithm configuration.
#' @return a vector of length \code{d} containing the compute lambda sequence.
#' @author Martin Vincent
#' @export
#' @import Matrix
sgl_lambda_sequence <- function(
	module_name,
	PACKAGE,
	data,
	parameterGrouping = NULL,
	groupWeights = NULL,
	parameterWeights = NULL,
	alpha,
	d = 100,
	lambda.min,
	algorithm.config = sgl.standard.config,
	lambda.min.rel = FALSE) {

	if(lambda.min <= 0) stop("lambda.min should be larger than zero")

	# cast
	if( is.null(d) ) {
		stop("d is NULL")
	}

	d <- as.integer(d)

	# Prepare arguments
	args <- prepare.args(
		data = data,
		parameterGrouping = parameterGrouping,
		groupWeights = groupWeights,
		parameterWeights = parameterWeights,
		alpha = alpha
	)

	call_sym <- get(paste(module_name, "sgl_lambda", "R", sep="_"),
	                asNamespace(PACKAGE))

	res <- do.call(call_sym, args = 
	                 list(data = args$data,
	                      block_dim = args$block_dim,
	                      groupWeights = args$groupWeights,
	                      parameterWeights = args$parameterWeights,
	                      alpha = args$alpha,
	                      d = d,
	                      lambda.min = lambda.min,
	                      lambda.min.rel = lambda.min.rel,
	                      algorithm.config = algorithm.config)
	)

	if( any(is.na(res)) ) stop("Can not compute lambda max. Review data and penalty settings.")

	if(res[1] < res[length(res)]) stop(paste("lamdba.min should be smaller than lambda.max (=", round(res[1],4),")", sep=""))

	return(res)
}
