#
#     Description of this R script:
#     R interface rotine for fitting a sparse group lasso regularization path
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

#' @title Fit a Sparse Group Lasso Regularization Path.
#'
#' @description
#' Computes a sequence of minimizers (one for each lambda given in the \code{lambda} argument) of
#' \deqn{\mathrm{loss}(\beta) + \lambda \left( (1-\alpha) \sum_{J=1}^m \gamma_J \|\beta^{(J)}\|_2 + \alpha \sum_{i=1}^{n} \xi_i |\beta_i| \right)}
#' where \eqn{\mathrm{loss}} is the loss/objective function specified by \code{module_name}.
#' The parameters are organized in the parameter matrix \eqn{\beta} with dimension \eqn{q\times p}.
#' The vector \eqn{\beta^{(J)}} denotes the \eqn{J} parameter group.
#' The group weights \eqn{\gamma \in [0,\infty)^m} and the parameter weights \eqn{\xi = (\xi^{(1)},\dots, \xi^{(m)}) \in [0,\infty)^n}
#' with \eqn{\xi^{(1)}\in [0,\infty)^{n_1},\dots, \xi^{(m)} \in [0,\infty)^{n_m}}.
#'
#' @param module_name reference to objective specific C++ routines.
#' @param PACKAGE name of the calling package.
#' @param data a list of data objects -- will be parsed to the specified module.
#' @param parameterGrouping grouping of parameters, a vector of length \eqn{p}. Each element of the vector specifying the group of the parameters in the corresponding column of \eqn{\beta}.
#' @param groupWeights the group weights, a vector of length \code{length(unique(parameterGrouping))} (the number of groups).
#' @param parameterWeights a matrix of size \eqn{q \times p}.
#' @param alpha the \eqn{\alpha} value 0 for group lasso, 1 for lasso, between 0 and 1 gives a sparse group lasso penalty.
#' @param lambda lambda.min relative to lambda.max (if \code{compute_lambda = TRUE}) or the lambda sequence for the regularization path, a vector or a list of vectors (of the same length) with the lambda sequence for the subsamples.
#' @param d length of lambda sequence (ignored if \code{compute_lambda = FALSE})
#' @param compute_lambda should the lambda sequence be computed
#' @param return_indices the indices of lambda values for which to return fitted parameters.
#' @param algorithm.config the algorithm configuration to be used.
#' @return
#' \item{Y.true}{the response, that is the \code{y} object in data as created by \code{create.sgldata}.}
#' \item{beta}{the fitted parameters -- a list of length \code{length(return)} with each entry a matrix of size \eqn{q\times (p+1)} holding the fitted parameters.}
#' \item{loss}{the values of the loss function.}
#' \item{objective}{the values of the objective function (i.e. loss + penalty).}
#' \item{lambda}{the lambda values used.}
#' @author Martin Vincent
#' @export
#' @useDynLib sglOptim, .registration=TRUE
#' @importFrom utils packageVersion
#' @import Matrix
sgl_fit <- function(module_name, PACKAGE,
	data,
	parameterGrouping = NULL,
	groupWeights = NULL,
	parameterWeights = NULL,
	alpha,
	lambda,
	d = 100,
	compute_lambda = length(lambda) == 1,
	return_indices = NULL,
	algorithm.config = sgl.standard.config) {

	# Compute lambda sequence
	if( compute_lambda ) {

		if( length(lambda) != 1 || lambda > 1 || lambda < 0) {
			stop("lambda must be a single number in the range (0,1) ")
		}

		if( length(d) != 1 || as.integer(d) != d || d < 1) {
			stop("d must be a single integer larger than 1")
		}

		lambda <- sgl_lambda_sequence(
			module_name = module_name,
			PACKAGE = PACKAGE,
			data = data,
			parameterGrouping = parameterGrouping,
			groupWeights = groupWeights,
			parameterWeights = parameterWeights,
			alpha = alpha,
			d = d,
			lambda.min = lambda,
			algorithm.config = algorithm.config,
			lambda.min.rel = TRUE
		)

	}

	# Prapare arguments
	args <- prepare.args(
		data = data,
		parameterGrouping = parameterGrouping,
		groupWeights = groupWeights,
		parameterWeights = parameterWeights,
		alpha = alpha
	)

	if( is.null(return_indices) ) {
		return_indices <- 1:length(lambda)
	}

	idx <- as.integer(sort(unique(return_indices))) - 1L
	if( any(idx < 0) || any(idx >= length(lambda)) ) {
		stop("return_indices indvalid")
	}

	call_sym <- paste(module_name, "sgl_fit", sep="_")
	res <- .Call(call_sym, PACKAGE = PACKAGE,
		args$data,
		args$block_dim,
		args$groupWeights,
		args$parameterWeights,
		args$alpha,
		lambda,
		idx,
		algorithm.config
	)

	# Add true response
	res$Y.true <- data$data$Y

	## Create R sparse matrix
	res$beta <- lapply(1:length(res$beta), function(i) sparseMatrix_from_C_format(res$beta[[i]]))

	# Dim names
	if( ! is.null(args$parameterNames) ) {
		res$beta <- lapply(res$beta, function(x) { dimnames(x) <- args$parameterNames; x })
	}

	# Restore org order
	res$beta <- lapply(res$beta, function(x) x[, order(args$group_order), drop = FALSE])

	res$nmod <- length(res$beta)

	# Set version, type and class and return
	res$module_name <- module_name
	res$PACKAGE <- PACKAGE
	res$sglOptim_version <- packageVersion("sglOptim")
	res$type <- "fit"
	class(res) <- "sgl"

	return(res)
}
