#
#     Description of this R script:
#     R interface to sgl-predict
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

#' @title Predict
#'
#' @description Predict and return responses as defined in the module.
#'
#' @details If no formating is done (i.e. if \code{responses = NULL})
#' then the \code{responses} field contains a list of lists structured in the following way:
#'
#'\itemize{
#' \item{sample 1}{
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
#' \item{sample 2}{
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
#'
#' If \code{responses = "rname"} with \code{rname} the name of the response then a list at \code{responses$rname} will be returned.
#' The content of the list will depend on the type of the response.
#' \itemize{
#'\item {\emph{scalar:} a matrix of size \eqn{n \times d} with the responses (where \eqn{n} is the number of samples and \eqn{d} the length of the lambda sequence).}
#'\item {\emph{vector:} a list of length \eqn{d} with each element a matrix of dimension \eqn{n \times q} containing the responses for the corresponding model (where \eqn{q} is the dimension of the response).}
#'\item {\emph{matrix:} a list with format samples -> models - > the response matrix.}
#'}
#'
#' @param module_name reference to objective specific C++ routines.
#' @param PACKAGE name of the calling package.
#' @param object a sgl object containing a list of estimated models.
#' @param data a list of data objects -- will be parsed to the specified module.
#' @param responses a vector of responses to simplify and return (if NULL (deafult) no formating will be done)
#' @param auto_response_names set response names
#' @param ... not used.
#' @return
#' \item{responses}{list of lists structured as described in details.
#' Content of the response elements will depend on the C++ response class}
#' \item{lambda}{the lambda sequence used.}
#' @author Martin Vincent
#' @useDynLib sglOptim, .registration=TRUE
#' @importFrom utils packageVersion
#' @importFrom methods as
#' @export
sgl_predict <- function(
	module_name,
	PACKAGE,
	object,
	data,
	responses = NULL,
	auto_response_names = TRUE,...) {

	# sparse X format
	if(data$sparseX) {
		data$X <- as(data$data$X, "CsparseMatrix")
		data$X <- list(dim(data$X), data$X@p, data$X@i, data$X@x)
	} else {
		data$X <- as.matrix(data$data$X)
	}

	if("beta" %in% names(object)) {

		beta <- lapply(X = object$beta, FUN = function(m) as(m, "CsparseMatrix"))
		beta <- lapply(X = beta, FUN = function(m) list(dim(m), m@p, m@i, m@x))

		call_sym <- paste(module_name, "sgl_predict", sep="_")
		response_list <- .Call(call_sym, PACKAGE = PACKAGE, data, beta)

	} else  {
    stop("No models found -- missing beta")
	}

	# Set response names
	if(auto_response_names) {
		response_list <- .set_response_names(response_list, data)
	}

	# set sample names
	names(response_list) <- data$sample_names
	attr(response_list, "type") <- c("sample", "model", "response")

	if( ! is.null(responses) ) {
		response_list <- lapply(responses, function(x)
			.format_response(response_list, x))
		names(response_list) <- responses
	}

	res <- list()

	res$responses <- response_list
	res$lambda <- object$lambda
	res$sample.names <- data$sample.names
	res$n_samples <- data$n_samples

	# Set version, type and class and return
	res$module_name <- module_name
	res$PACKAGE <- PACKAGE
	res$sglOptim_version <- packageVersion("sglOptim")
	res$type <- "predict"
	class(res) <- "sgl"

	return(res)
}
