#
#     Description of this R script:
#     Routines for navigating sgl objects
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


#' @title Generic function for computing error rates
#'
#' @description
#' Compute and returns an error rate for each model contained in \code{x}.
#' See details for generic use cases.
#'
#' @details
#' The following generic use case should be supported (see for example \pkg{msgl} package for an implementation):
#'
#' \enumerate{
#' \item With \code{fit} a sgl fit object with models estimated using \code{x} data, the code
#'
#' \code{Err(fit, x)}
#'
#' should return a vector with the \emph{training errors} of the models.
#'
#' \item With \code{x.new} a new data set with known responses \code{response.new}, the code
#'
#' \code{Err(fit, x.new, response.new)}
#'
#' should return a vector with the errors of the models when applied to the new data set.
#
#' \item With \code{fit.cv} a sgl cross validation object, the code
#'
#' \code{Err(fit.cv)}
#'
#' should return a vector with estimates of the \emph{expected generalization errors} of the models (i.e. the cross validation errors).
#'
#' \item If subsampling is supported then, with \code{fit.sub} a sgl subsampling object, the code
#'
#' \code{Err(fit.sub)}
#'
#' should return a matrix with the test errors (each column corresponding to a model, i.e. rows corresponds to tests).
#' }
#'
#' @seealso compute_error
#' @param object an object
#' @param data a data object
#' @param response a response object
#' @param ... additional parameters (optional)
#' @return
#' a vector of length \code{nmod(object)} or a matrix with \code{nmod(object)} columns containing error rates for the models
#' @author Martin Vincent
#' @export
Err <- function(object, data, response, ... ) UseMethod("Err")

#' @title Error Rates
#'
#' @description
#' Compute and return the root-mean-square error for each model.
#' This method is only intended for testing.
#'
#' The root-mean-square error (RMSE) is
#' \deqn{\frac{1}{K}\sum_{i = 1}^K \sqrt{\frac{1}{N}\sum_{j=1}^N Y_{ji}-(X\hat \beta)_{ji}}}
#' RMSE is the default error.
#'
#' @param object a lsgl object.
#' @param data a design matrix (the \eqn{X} matrix).
#' @param response a matrix of the true responses (the \eqn{Y} matrix).
#' @param ... ignored.
#' @return a vector of errors.
#'
#' @author Martin Vincent
#' @method Err sgl
#' @export
Err.sgl <- function(object, data = NULL, response = object$Y.true, ... ) {

	loss = function(x,y) mean(sapply(1:length(x), function(i) sqrt(mean((x[[i]] - y[[i]])^2))))

	true_response <- response

	if( ! is.null(data) ) {

		data <- create.sgldata(data, NULL, sparseY = FALSE)

		object <- sgl_predict(
			module_name = object$module_name,
			PACKAGE = object$PACKAGE,
			object = object,
			data = data
		)
	}

	return( compute_error(
		x = object,
		response_name = "link",
		true_response = true_response,
		loss = loss)
	)
}


#' @title Helper function for computing error rates
#' @description
#' This function can be used to compute mean error rates.
#' It is consist with the use cases of the \code{Err} genetic function.
#' The loss function should be of the form
#' \code{function(x, y)}
#' and must return a single numeric number, with x a list of true responses and y a list of responses (one list element for each sample).
#' @param x sgl object containing responses
#' @param response_name the name of the response.
#' @param true_response the true response
#' @param loss the loss function.
#' @param transposed_response have the response list been transposed with \code{transpose_response_elements}
#' @return a vector or matrrix with the computed error rates
#'
#' @author Martin Vincent
#' @importFrom stats predict
#' @export
compute_error <- function(x, response_name, true_response, loss, transposed_response = FALSE) {

	if( is.null(response_name) ) {
		stop("no response_name given")
	}

	if( is.null(true_response) && ! is.null(x$Y.true) ) {
		true_response <- x$Y.true
	} else if( is.null(true_response) ) {
		stop("no true response given")
	}

	if( any(names(x) == response_name) ) {
		r <- x[[response_name]]
	} else if(any(names(x$responses) == response_name) ) {
		r <- x$responses[[response_name]]
	} else if( ! is.null(x$responses) ){
		r <- .format_response(x$responses, response_name)
	} else {
		stop("no response given")
	}

	type <- attr(r, "type")
	if( type[1] == "subsample" ) {

		return(
			t(sapply(1:length(r), function(s)
				.loop_loss(true_response[[s]], r[[s]], type[-1], loss, transposed_response)
			))
		)

	} else {

		return( .loop_loss(true_response, r, type, loss, transposed_response) )

	}
}

.loop_loss <- function(y, r, type, loss, transposed_response) {

	if( is.list(y) ) {
		# do nothing
	} else if( is.matrix(y) || is(y, "sparseMatrix") ) {
		y <- lapply(1:nrow(y), function(i) y[i,])
	} else if( is.vector(y) || is.factor(y) ) {
		y <- as.list(as.vector(y))
	} else {
		stop("unknown true response type")
	}

	if( type[1] == "sample" && type[2] == "model" ) {

		r <- lapply(1:length(r[[1]]), function(i)
			lapply(1:length(r), function(j) r[[j]][[i]])
		)

	} else if( type[1] == "model" && is.matrix(r[[1]]) && ! transposed_response ) {

		r <- lapply(1:length(r), function(i)
			lapply(1:nrow(r[[1]]), function(j) r[[i]][j,])
		)

	} else if( type[1] == "model" && is.matrix(r[[1]]) && transposed_response ) {

		r <- lapply(1:length(r), function(i)
			lapply(1:ncol(r[[1]]), function(j) r[[i]][,j])
		)

	} else if( type[1] == "response" && is.matrix(r) && ! transposed_response ) {

		r <- lapply(1:ncol(r), function(i)
			lapply(1:nrow(r), function(j) r[j,i])
		)

	} else if( type[1] == "response" && is.matrix(r) && transposed_response ) {

		r <- lapply(1:nrow(r), function(i)
			lapply(1:ncol(r), function(j) r[i,j])
		)

	} else {
		stop("unknown response type")
	}

	return(
		sapply(1:length(r), function(i)
			loss(y, r[[i]])
		)
	)
}
