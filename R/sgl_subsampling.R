#
#     Description of this R script:
#     R interface to generic sparse group lasso subsampling procedure
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

# Tell R that 'task' (used in foreach) is global variable -- R check will complain if not
globalVariables('task')

#' @title Generic sparse group lasso subsampling procedure
#'
#' @description Subsampling procedure with support parallel computations.
#' @details If no formating is done (i.e. if \code{responses = NULL})
#' then the \code{responses} field contains a list of lists structured in the following way:
#'
#'subsamples 1:
#'\itemize{
#' \item{sample \code{test[[1]][1]}}{
#' 	\itemize{
#'	 \item{model (lambda) index 1}{
#'   	\itemize{
#'       \item{}{response elements}
#'		 }}
#'	 \item{model (lambda) index 2}{
#'   	\itemize{
#'       \item{}{response elements}
#'		 }}
#'	 \item{...}{}
#'	}}
#' \item{sample \code{test[[1]][2]}}{
#' 	\itemize{
#'	 \item{model (lambda) index 1}{
#'   	\itemize{
#'       \item{}{response elements}
#'		 }}
#'	 \item{model (lambda) index 2}{
#'   	\itemize{
#'       \item{}{response elements}
#'		 }}
#'	 \item{...}{}
#'  }}
#' \item{...}{}
#' }
#'subsamples 2:
#'  ...
#'
#' If \code{responses = "rname"} with \code{rname} the name of the response then a list at \code{responses$rname} will be returned.
#' The content of the list will depend on the type of the response.
#' \itemize{
#	\item{scalar}{ A list of subsamples (of length \code{length(test)}) containing matrices of size \eqn{n_i \times d} with the responses (where \eqn{n_i} is the number of samples in subsample \eqn{i} (that is \code{length(test[[i]])}) and \eqn{d} the length of the lambda sequence).}
#'\item{vector}{ A list with format subsamples -> models -> matrix of dimension \eqn{n_i \times q} containing the responses for the corresponding model and subsample (where \eqn{q} is the dimension of the response).}
#'\item{matrix}{ A list with format subsamples -> samples -> models - > the response matrix.}
#' }
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
#' @param training a list of training samples, each item of the list corresponding to a subsample.
#' Each item in the list must be a vector with the indices of the training samples for the corresponding subsample.
#' The length of the list must equal the length of the \code{test} list.
#' @param test a list of test samples, each item of the list corresponding to a subsample.
#' Each item in the list must be vector with the indices of the test samples for the corresponding subsample.
#' The length of the list must equal the length of the \code{training} list.
#' @param responses a vector of responses to simplify and return (if NULL (deafult) no formating will be done)
#' @param auto_response_names set response names
#' @param collapse if \code{TRUE} the results will be collapsed and ordered into one result, resembling the output of \code{sgl_predict}  (this is only valid if the test samples are not overlapping)
#' @param max.threads Deprecated (will be removed in 2018),
#' instead use \code{use_parallel = TRUE} and registre parallel backend (see package 'doParallel').
#' The maximal number of threads to be used.
#' @param use_parallel If \code{TRUE} the \code{foreach} loop will use \code{\%dopar\%}. The user must registre the parallel backend.
#' @param algorithm.config the algorithm configuration to be used.
#' @return
#' \item{Y.true}{the response, that is the \code{y} object in data as created by \code{create.sgldata}.}
#' \item{responses}{content will depend on the C++ response class}
#' \item{features}{number of features used in the models}
#' \item{parameters}{number of parameters used in the models}
#' \item{lambda}{the lambda sequences used (a vector or list of length \code{length(training)}).}
#' @author Martin Vincent
#' @useDynLib sglOptim, .registration=TRUE
#' @importFrom utils packageVersion
#' @importFrom foreach foreach %do% %dopar%
#' @importFrom doParallel registerDoParallel
#' @importFrom parallel makeCluster stopCluster
#' @export
sgl_subsampling <- function(
  module_name,
  PACKAGE,
  data,
  parameterGrouping = NULL,
  groupWeights = NULL,
  parameterWeights = NULL,
  alpha,
  lambda,
  d = 100,
  compute_lambda = length(lambda) == 1,
  training = NULL,
  test = NULL,
  responses = NULL,
  auto_response_names = TRUE,
  collapse = FALSE,
  max.threads = NULL,
  use_parallel = FALSE,
  algorithm.config = sgl.standard.config) {

	# deprecated warnings
	if( ! is.null(max.threads) ) {
		warning("Argument 'max.threads' is deprecated (will be removed in 2018). \n    Alternative: Registre parallel backend (see package doParallel) and use 'use_parallel = TRUE' ")
	}

	# Check training and test consistency:
	if( ! is.list(training) | ! is.list(test)) {
		stop("The arguments traning and test should be lists")
	}

	if(length(training) != length(test)) {
		stop("The length of the lists traning and test should be equal")
	}

	# Check if collapse = TRUE is valid
	if(	collapse == TRUE) {
		if( length(unique(unlist(test))) != length(unlist(test)) ) {
			stop("collapse = TRUE only valid if test samples are not overlapping")
		}
	}

	training <- lapply(training, sort)
	test <- lapply(test, sort)

	# Make lambda a list and validate
	if(is.list(lambda)) {

		if( ! all(sapply(lambda, function(x) length(x) == length(lambda[[1]])))) {
			stop("all lambda sequences must have same length")
		}

		if(length(lambda) != length(training)) {
			stop("The length of the lists lambda, traning and test should be equal")
		}

		lambda.list <- lambda

	} else if ( ! compute_lambda && is.numeric(lambda) ){

		lambda.list <- replicate(length(training), lambda, simplify = FALSE)

	} else if ( is.numeric(lambda) ) {

		# Compute lambda sequence

		if( length(lambda) != 1 || lambda > 1 || lambda < 0) {
			stop("lambda must be a single number in the range (0,1) ")
		}

		if( length(d) != 1 || as.integer(d) != d || d < 1) {
			stop("d must be a single integer larger than 1")
		}

		lambda.list <- lapply(training, function(idx) sgl_lambda_sequence(
			module_name = module_name,
			PACKAGE = PACKAGE,
			data =  subsample(data, idx),
			parameterGrouping = parameterGrouping,
			groupWeights = groupWeights,
			parameterWeights = parameterWeights,
			alpha = alpha,
			d = d,
			lambda.min = lambda,
			algorithm.config = algorithm.config,
			lambda.min.rel = TRUE)
		)

	} else {
    stop("lambda must be numeric or a list of numerics")
  }

	call_sym <- paste(module_name, "sgl_subsampling", sep="_")

	# Registre parallel backend
	# This is only to make max.threads work -  remov in 2018
	if( ! is.null(max.threads) && max.threads > 1) {
		cl <- makeCluster(max.threads)
		registerDoParallel(cl)
	}

	if( use_parallel || ( ! is.null(max.threads) && max.threads != 1 ) ) {

		rawres <- foreach(task=1:length(training),
			.packages = PACKAGE) %dopar% {

      test_data <- subsample(data, test[[task]])
			train_data <- subsample(data, training[[task]])

			# Prapare arguments
			args <- prepare.args(
				data = train_data,
				parameterGrouping = parameterGrouping,
				groupWeights = groupWeights,
				parameterWeights = parameterWeights,
				alpha = alpha,
				test_data = test_data)

			x <- .Call(call_sym, PACKAGE = PACKAGE,
				args$data,
				args$test_data,
				args$block_dim,
				args$groupWeights,
				args$parameterWeights,
				args$alpha,
				lambda.list[[task]],
				algorithm.config
			)

			names(x$responses) <- test_data$sample_names

			return(x)
		}

	} else {

		rawres <- foreach(task=1:length(training)) %do% {

    test_data <- subsample(data, test[[task]])
	  train_data <- subsample(data, training[[task]])

		# Prapare arguments
		args <- prepare.args(
			data = train_data,
			parameterGrouping = parameterGrouping,
			groupWeights = groupWeights,
			parameterWeights = parameterWeights,
			alpha = alpha,
			test_data = test_data)

		x <- .Call(call_sym, PACKAGE = PACKAGE,
			args$data,
			args$test_data,
			args$block_dim,
			args$groupWeights,
			args$parameterWeights,
			args$alpha,
			lambda.list[[task]],
			algorithm.config
		)

		names(x$responses) <- test_data$sample_names

		return(x)
		}
	}

	if( ! is.null(max.threads) && max.threads > 1) {
 		stopCluster(cl)
	}

	response_list <- lapply(rawres, function(x) x$responses)

	# Set response names
	if(auto_response_names) {
		response_list <- lapply(response_list, function(rl) .set_response_names(rl, data))
	}

	attr(response_list, "type") <- c("subsample", "sample", "model", "response")


	if( collapse ) {

		response_list <- unlist(response_list, recursive = FALSE)
		response_list <- response_list[order(unlist(test))]
		attr(response_list, "type") <- c("sample", "model", "response")

	}

	if( ! is.null(responses) ) {

		response_list <- lapply(responses, function(x)
			.format_response(response_list, x)
		)

		names(response_list) <- responses
	}

	# Build result list
	res <- list()

	# Add true response
	if(collapse) {
		res$Y.true <- data$data$Y
	} else {
		if( is.matrix(data$data$Y) || is(data$data$Y, "sparseMatrix") ) {
			res$Y.true <- lapply(test, function(idx) data$data$Y[idx,,drop=FALSE])
		} else if ( is.vector(data$data$Y) || is.factor(data$data$Y) ) {
			res$Y.true <- lapply(test, function(idx) data$data$Y[idx])
		} else {
			stop(paste("Can not handle response class '", class(data$data$Y), "'.", sep =""))
		}
	}

  # Add responses
	res$responses <- response_list

	res$features <- t(sapply(rawres, function(x) x$features))
	rownames(res$features) <- paste("sample-set", 1:nrow(res$features), sep="-")

	res$parameters <- t(sapply(rawres, function(x) x$parameters))
	rownames(res$parameters) <- paste("sample-set", 1:nrow(res$parameters), sep="-")

	res$lambda <- lambda.list

	res$collapse <- collapse

	# Set version, type and class and return
	res$module_name <- module_name
	res$PACKAGE <- PACKAGE
	res$sglOptim_version <- packageVersion("sglOptim")
	res$type <- "subsampling"
	class(res) <- "sgl"

	return(res)
}
