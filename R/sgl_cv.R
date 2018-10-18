#
#     Description of this R script:
#     R interface generic sparse group lasso cross validation
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

#' Generic sparse group lasso cross validation using multiple possessors
#'
#'
#' @param module_name reference to objective specific C++ routines.
#' @param PACKAGE name of the calling package.
#' @param data a list of data objects -- will be parsed to the specified module.
#' @param parameterGrouping grouping of parameters, a vector of length \eqn{p}.
#' Each element of the vector specifying the group of the parameters in the corresponding column of \eqn{\beta}.
#' @param groupWeights the group weights, a vector of length \code{length(unique(parameterGrouping))} (the number of groups).
#' @param parameterWeights a matrix of size \eqn{q \times p}.
#' @param alpha the \eqn{\alpha} value 0 for group lasso, 1 for lasso, between 0 and 1 gives a sparse group lasso penalty.
#' @param lambda lambda.min relative to lambda.max (if \code{compute_lambda = TRUE}) or the lambda sequence for the regularization path,
#' a vector or a list of vectors (of the same length) with the lambda sequence for the subsamples.
#' @param d length of lambda sequence (ignored if \code{compute_lambda = FALSE})
#' @param compute_lambda should the lambda sequence be computed
#' @param fold the fold of the cross validation, an integer larger than \eqn{1} and less than \eqn{N+1}.
#' Ignored if \code{cv.indices != NULL}.
#' If \code{fold}\eqn{\le}\code{max(table(classes))} then the data will be split into \code{fold} disjoint subsets keeping the ration of classes approximately equal.
#' Otherwise the data will be split into \code{fold} disjoint subsets without keeping the ration fixed.
#' @param sampleGroups grouping of samples,
#' the algorithm computing the cv.indices will try to equally divide the groups among the subsamples.
#' @param cv.indices a list of indices of a cross validation splitting.
#' If \code{cv.indices = NULL} then a random splitting will be generated using the \code{fold} argument.
#' @param responses a vector of responses to simplify and return (if NULL (deafult) no formating will be done)
#' @param max.threads Deprecated (will be removed in 2018),
#' instead use \code{use_parallel = TRUE} and registre parallel backend (see package 'doParallel').
#' The maximal number of threads to be used.
#' @param use_parallel If \code{TRUE} the \code{foreach} loop will use \code{\%dopar\%}.
#' The user must registre the parallel backend.
#' @param algorithm.config the algorithm configuration to be used.
#' @return
#' \item{Y.true}{the response, that is the \code{y} object in data as created by \code{create.sgldata}.}
#' \item{responses}{content will depend on the C++ response class}
#' \item{cv.indices}{the cross validation splitting used}
#' \item{features}{number of features used in the models}
#' \item{parameters}{number of parameters used in the models}
#' \item{lambda}{the lambda sequence used.}
#' @author Martin Vincent
#' @importFrom utils packageVersion
#' @export
sgl_cv <- function(module_name, PACKAGE,
	data,
	parameterGrouping = NULL,
	groupWeights = NULL,
	parameterWeights = NULL,
	alpha,
	lambda,
	d = 100,
	compute_lambda = length(lambda) == 1,
	fold = 2,
	sampleGroups = NULL,
	cv.indices = list(),
	responses = NULL,
	max.threads = NULL,
	use_parallel = FALSE,
	algorithm.config = sgl.standard.config) {

	# cast
	fold <- as.integer(fold)

	if(length(cv.indices) == 0) {

		# Check fold
		if(fold < 2) {
			stop("fold must be equal to or larger than 2")
		}

		if(fold > data$n_samples) {
			stop("fold must be equal to or less than the number of samples")
		}

		if( is.null(sampleGroups)  || fold > min(table(sampleGroups)) ) {

			cv.indices <- sample(1:data$n_samples)
			cv.indices <- lapply(1:fold, function(i) cv.indices[(1:data$n_samples %% fold) == (i-1)])

		} else {

			# compute cv indices
			cv.indices <- lapply(unique(sampleGroups), function(x) .divide_indices(which(sampleGroups == x), fold))
			cv.indices <- lapply(cv.indices, function(x) sample(x))
			cv.indices <- lapply(1:fold, function(i) sort(unlist(lapply(cv.indices, function(x) x[[i]]))))

		}

		# Chek consistency of cv.indices
		if(length(unlist(cv.indices)) != data$n_samples || sum(duplicated(unlist(cv.indices))) > 0) {
			stop("Internal error computing the cross validation splitting (this is a bug)")
		}

	} else {
		# use user supplied cv splitting
		# Chek consistency of cv.indices
		if(length(cv.indices) == 1 || length(unlist(cv.indices)) != data$n_samples || sum(duplicated(unlist(cv.indices))) > 0) {
			stop("User supplied cv.indices are invalid \n (the cv.indices does not represent a cross validation splitting, use subsampling for general subsampling)")
		}
	}

	samples <- 1:max(unlist(cv.indices))
	training <- lapply(cv.indices, function(x) samples[-x])
	test <- cv.indices

 # Run subsampling
	res <- sgl_subsampling(module_name, PACKAGE,
		data = data,
		parameterGrouping = parameterGrouping,
		groupWeights = groupWeights,
		parameterWeights = parameterWeights,
		alpha = alpha,
		lambda = lambda,
		d = d,
		compute_lambda = compute_lambda,
		training = training,
		test = test,
		responses = responses,
		collapse = TRUE,
		max.threads = max.threads,
		use_parallel = use_parallel,
		algorithm.config = algorithm.config)

	# Add cv.indices
	res$cv.indices <- cv.indices

	# Set version, type and class and return
	res$module_name <- module_name
	res$PACKAGE <- PACKAGE
	res$sglOptim_version <- packageVersion("sglOptim")
	res$type <- "cv"
	class(res) <- "sgl"
	return(res)
}

.divide_indices <- function(indices, fold) {

	if(fold > length(indices)) {
		stop("fold large than length of indices vector")
	}

	tmp <- sapply(0:(length(indices)-1), function(i) (i %% fold)+1)
	return(lapply(1:fold, function(i) indices[tmp == i]))
}
