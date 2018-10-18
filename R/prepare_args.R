#
#     Description of this R script:
#     Routines for handling sgl-data
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


#' @title Generic rearrange function
#'
#' @description
#' Rearrange the order of the covariates in the \code{data} object.
#'
#' @param data a data object
#' @param covariate.order the new order of the covariates
#' @param ... additional parameters
#' @return a rearranged data object of same class as \code{data}
#'
#' @seealso rearrange.sgldata
#' @author Martin Vincent
#' @export
rearrange <- function(data, covariate.order, ...) UseMethod("rearrange")

#' @title Generic function for preparing the sgl call arguments
#'
#' @description
#' Compute and prepare the sgl call arguments for the objective function
#' \deqn{\mathrm{loss}(\mathrm{data})(\beta) + \lambda \left( (1-\alpha) \sum_{J=1}^m \gamma_J \|\beta^{(J)}\|_2 + \alpha \sum_{i=1}^{n} \xi_i |\beta_i| \right)}
#' where \eqn{\mathrm{loss}} is a loss/objective function.
#' The \eqn{n} parameters are organized in the parameter matrix \eqn{\beta} with dimension \eqn{q\times p}.
#' The vector \eqn{\beta^{(J)}} denotes the \eqn{J} parameter group, the dimension of \eqn{\beta^{(J)}} is denote by \eqn{d_J}.
#' The dimensions \eqn{d_J} must be multiple of \eqn{q}, and \eqn{\beta = (\beta^{(1)} \cdots \beta^{(m)})}.
#' The group weights \eqn{\gamma \in [0,\infty)^m} and the parameter weights \eqn{\xi \in [0,\infty)^{qp}}.
#' @param data a data object
#' @param ... additional parameters
#' @return
#' \item{block_dim}{a vector of length \eqn{m}, containing the dimensions \eqn{d_J} of the groups (i.e. the number of parameters in the groups)}
#' \item{groupWeights}{a vector of length \eqn{m}, containing the group weights}
#' \item{parameterWeights}{a matrix of dimension \eqn{q \times p}, containing the parameter weights}
#' \item{alpha}{the \eqn{\alpha} value}
#' \item{data}{the data parsed to the loss module}
#' \item{group_order}{original order of the columns of \eqn{\beta}. Before sgl routines return \eqn{\beta} will be reorganized according to this order.}
#'
#' @seealso prepare.args.sgldata
#' @author Martin Vincent
#' @export
#' @family sgldata
prepare.args <- function(data, ...) UseMethod("prepare.args")

#' @title Subsample
#'
#' @description
#' Pick out a subsample of an object
#'
#' @param data a data object
#' @param indices a vector of indices to pick out
#' @param ... not used
#' @return a data object of the same class as \code{data}
#' @author Martin Vincent
#' @export
subsample <- function(data, indices, ...) UseMethod("subsample")


#' @title Add data to a sgldata data object
#'
#' @description
#' Addes a data to a sgldata data object
#'
#' @param data sgldata object
#' @param x data object to add,
#' @param name name of data object
#' @param ... additional parameters
#' @author Martin Vincent
#' @export
add_data <- function(data, x, name, ...) UseMethod("add_data")

#' @title Rearrange sgldata
#'
#' @description
#' Rearrange the order of the covariates in a sgldata object.
#'
#' @param data  a sgldata object
#' @param covariate.order the new order of the covariates
#' @param ... not used
#' @return a sgldata object with the covariates reordered
#' @author Martin Vincent
#' @method rearrange sgldata
#' @export
#' @family sgldata
rearrange.sgldata <- function(data, covariate.order, ...)
{

	data$data$X <- data$data$X[,covariate.order, drop = FALSE]
	data$covariate.names <- data$covariate.names[covariate.order]

	return(data)
}

#' @title Subsample sgldata
#'
#' @description
#' Pick out a subsample of a sgldata object
#'
#' @param data  a sgldata object
#' @param indices a vector of indices to pick out
#' @param ... not used
#' @return a sgldata
#' @author Martin Vincent
#' @importFrom methods is
#' @method subsample sgldata
#' @export
#' @family sgldata
subsample.sgldata <- function(data, indices, ...)  {

  if( is.character(indices) ) {
    if( ! all( indices %in% data$sample_names ) ) {
      stop("indices not found in data")
    }

    data$sample_names <- indices

  } else {
    data$sample_names <- data$sample_names[indices]
  }

  data$data <- lapply(data$data, function(x) {

    if( is.null(x) ) {

      # do nothing

    } else if( is.vector(x) ) {

      if(is.character(indices) && ! all( indices %in% names(x) )) {
        stop("indices not found in data")
      }

      return( x[indices] )

    } else if( is.matrix(x) || is(x, "Matrix") ) {

      if(is.character(indices) && ! all( indices %in% rownames(x) )) {
        stop("indices not found in data")
      }

      return( x[indices, , drop = FALSE] )

    } else {

      stop(paste("Can not handle data of type:", class(x)))

    }
  })

	return(data)
}

#' @title Prepare a sgldata data object
#'
#' @description
#' Creates a sgldata data object from a matrix or vector
#'
#' @param x the matrix,
#' @param default default value, returned if x is null
#' @param type data type, 'numeric' or 'integer'
#' @param sparse if TRUE \code{y} will be treated as sparse, if FALSE \code{y} will be treated as dens.
#' @author Martin Vincent
#' @importFrom methods is
#' @importFrom methods as
#' @export
#' @family sgldata
prepare_data <- function(
	x,
	default = NULL,
	type = "numeric",
	sparse = is(x, "sparseMatrix")) {

	if( is.null(x) ) {
		return( default )
	}

	if( ! is.null(x) && any(is.na(x))) {
		stop(paste("data matrix contains NA values"))
	}

  if( type == "numeric" ) {

    if(sparse && (is.matrix(x) || is(x, "Matrix")) ) {

      return( as(x, "CsparseMatrix") )

		}	else if( is.vector(x) ) {

      element_names <- names(x)
      x <- as.numeric(x)
      names(x) <- element_names

      return( x )

		} else if( is.matrix(x) || is(x, "Matrix") ) {

      element_names <- dimnames(x)
      x <- as.matrix(x)
      x <- apply(x, c(1,2), as.numeric)
      dimnames(x) <- element_names

      return( x )

		} else {

      stop(paste("data matrix is of unknown type: ", class(x)))

    }
	}

  if( type == "integer" ) {

    if(sparse && (is.matrix(x) || is(x, "Matrix")) ) {

      return( as(x, "CsparseMatrix") )

    }	else if( is.vector(x) ) {

      element_names <- names(x)
      x <- as.integer(x)
      names(x) <- element_names

      return( x )

    } else if( is.matrix(x) || is(x, "Matrix") ) {

      element_names <- dimnames(x)
      x <- as.matrix(x)
      x <- apply(x, c(1,2), as.integer)
      dimnames(x) <- element_names

      return( x )

    } else {
      stop(paste("data matrix is of unknown type: ", class(x)))

    }
  }

	if(type == "factor") {

    element_names <- names(x)
    x <- as.integer(x) - 1L
    names(x) <- element_names

    return( x )

  }

  stop(paste("can not handle type '",type,"'",sep=""))
}

#' @title Create a sgldata object
#'
#' @description
#' Creates a sgldata object from a design matrix and an optional response vector or matrix.
#'
#' @param x the design matrix, a matrix of size \eqn{N \times p} (will be parsed to the loss module as \code{X}).
#' @param y the responses, \code{NULL}, a vector or a matrix (will be parsed to the loss module as matrix \code{Y})..
#' @param response_dimension number of models, that is the dimension of the returned response.
#' @param response_names names of models, that is the names of the elements of the returned response.
#' @param sparseX if TRUE \code{x} will be treated as sparse, if FALSE \code{x} will be treated as dens.
#' @param sparseY if TRUE \code{y} will be treated as sparse, if FALSE \code{y} will be treated as dens.
#' @param typeX type of the elements of x.
#' @param typeY type of the elements of y.
#' @author Martin Vincent
#' @importFrom methods is
#' @importFrom methods as
#' @export
#' @family sgldata
create.sgldata <- function(x, y,
	response_dimension = .get_response_dimension(y),
	response_names = .get_response_names(y),
	sparseX = is(x, "sparseMatrix"),
	sparseY = is(y, "sparseMatrix"),
	typeX = element_class(x),
	typeY = element_class(y)) {

	### domain checks
	if(any(is.na(x))) stop("design matrix contains NA values")
	if( ! is.null(y) && any(is.na(y))) stop("response contains NA values")

	### Create data object
	data <- list()

	### Dimensions
	data$n_covariate <- ncol(x)
	data$n_samples <- nrow(x)

	### Dim names
	data$sample_names <- rownames(x)
	data$covariate_names <- colnames(x)
	data$response_names <- response_names

	if(data$n_samples < 2) {
		stop("design matrix contains less than 2 samples")
	}

	data$response_dimension <- response_dimension

	class(data) <- "sgldata"

	### X data
	data <- add_data(data, x, "X", type = typeX, sparse = sparseX)

	### Y data
	if( is.vector(y) ) {
		y <- matrix(y, nrow = length(y), ncol = 1)
	}

	data <- add_data(data, y, "Y", type = typeY, sparse = sparseY)

	return( data )
}

.get_response_dimension <- function(x) {

	if( is.matrix(x) || is(x, "Matrix") ) {
		return(ncol(x))
	}

	if( is.vector(x) ) {
		return(1)
	}

	if( is.factor(x) ) {
		return (length(levels(x)))
	}

	if( is.null(x) ) {
		return( NULL )
	}

	stop(paste("Unknown response type:", class(x)))
}

.get_response_names <- function(x) {

	if( is.matrix(x) || is(x, "Matrix") ) {
		return(colnames(x))
	}

	if( is.vector(x) ) {
		return("response")
	}

	if( is.factor(x) ) {
		return (levels(x))
	}

	if( is.null(x) ) {
		return( NULL )
	}


	stop(paste("Unknown response type:", class(x)))
}

#' @title Add data to a sgldata data object
#'
#' @description
#' Addes a data to a sgldata data object
#'
#' @param data sgldata object
#' @param x matrix or vector,
#' @param name name of the data object
#' @param default default value, returned if x is null
#' @param type data type, 'numeric' or 'integer'
#' @param sparse if TRUE \code{y} will be treated as sparse, if FALSE \code{y} will be treated as dens.
#' @param ... not used
#' @author Martin Vincent
#' @export
#' @family sgldata
add_data.sgldata <- function(
  data,
  x,
  name,
  default = NULL,
  type = element_class(x),
  sparse = is(x, "sparseMatrix"), ... ) {

  if( is.null(data$data) ) {
    data$data <- list()
  }

  sparse_name <- paste("sparse", name, sep="")
  data[[sparse_name]] <- sparse

  data$data[[name]] <- prepare_data(
    x,
    default = default,
    type = type,
    sparse = sparse
  )

  if( is.null(data$data[[name]]) ) {
    return( data )
  }

  # check dimension
  if( ! is.null(data$data[[name]]) ) {
    if( is.vector(data$data[[name]]) ) {
      ns <- length(x)
    } else {
      ns <- nrow(data$data[[name]])
    }

    if(ns != data$n_samples) {
      stop("data dimension inconsistent")
    }
  }

  # Sample names
  if( ! is.null(data$sample_names) ) {
    if( is.vector(data$data[[name]]) ) {
      if(is.null(names(data$data[[name]]))) {
        names(data$data[[name]]) <- data$sample_names
      } else if( any(names(data$data[[name]]) != data$sample_names)) {
        stop("sample names mismatch")
      }
		} else {
      if(is.null(rownames(data$data[[name]]))) {
        rownames(data$data[[name]]) <- data$sample_names
      } else if( any(rownames(data$data[[name]]) != data$sample_names)) {
        stop("sample names mismatch")
      }
		}
  }

	return( data )
}

#' @title Prepare sgl function arguments
#'
#' @description
#' Prepare sgl function arguments using sgldata.
#'
#' @param data a sgldata object
#' @param parameterGrouping grouping of parameters, a vector of length \eqn{p}. Each element of the vector specifying the group of the parameters in the corresponding column of \eqn{\beta}.
#' @param groupWeights the group weights, a vector of length \code{length(unique(parameterGrouping))} (the number of groups).
#' @param parameterWeights a matrix of size \eqn{q \times p}, that is the same dimension as \eqn{\beta}.
#' @param parameterNames dim-names of parameters, if NULL \code{dimnames(parameterWeights)} will be used.
#' @param alpha the \eqn{\alpha} value 0 for group lasso, 1 for lasso, between 0 and 1 gives a sparse group lasso penalty.
#' @param test_data optional test data to be prepared (a sgldata object)
#' @param ... not used
#' @method prepare.args sgldata
#' @export
#' @family sgldata
#' @author Martin Vincent
prepare.args.sgldata <- function(
	data,
	parameterGrouping = NULL,
	groupWeights = NULL,
	parameterWeights = NULL,
	parameterNames = NULL,
	alpha,
	test_data = NULL, ...) {

	# If Lasso then ignore grouping
	if(alpha == 1) {
		parameterGrouping <- NULL
		groupWeights <- NULL
	}

	# Set deafult values if NULL
	if( is.null(parameterGrouping) ) {
		parameterGrouping <- 1:data$n_covariate
	}

	# ensure parameterGrouping is factor
	parameterGrouping <- factor(parameterGrouping)

	if( is.null(groupWeights) ) {
		groupWeights <- rep(1, length(levels(parameterGrouping)))
	}

	if( is.null(parameterWeights) ) {
		parameterWeights <- matrix(1, nrow = data$response_dimension, ncol = data$n_covariate)

		dimnames(parameterWeights) <- list(data$response_names, data$covariate_names)
	}

	### Compute block dim
	block_dim <-nrow(parameterWeights)*as.integer(table(parameterGrouping))

	### Prepare data

	# Reorder data
	group_order <- order(parameterGrouping)
	data <- rearrange(data, group_order)
	parameterWeights <- parameterWeights[,group_order, drop = FALSE]

	# Paramter names
	if( is.null(parameterNames) ) {
		parameterNames <- dimnames(parameterWeights)
	}

	### Create args list

	args <- list()

	args$block_dim <- block_dim
	args$groupWeights <- groupWeights
	args$parameterWeights <- parameterWeights
	args$parameterNames <- parameterNames
	args$alpha <- alpha
	args$data <- .format_data_list(data$data)
	args$group_order <- group_order

	### Prepare test data

	#NOTE check data and test_data consistency

	if( ! is.null(test_data) ) {

		# Reorder data
		test_data <- rearrange(test_data, group_order)

		args$test_data <- .format_data_list(test_data$data)
	}

	return(args)
}

#' @title Prepare sparse matrix for .Call
#'
#' @description
#' Prepare sparse matrix for .Call
#'
#' @param x a spares matrix
#' @author Martin Vincent
#' @export
sparseMatrix_to_C_format <- function(x) {
	x <- as(x, "CsparseMatrix")
	return( list(dim(x), x@p, x@i, x@x) )
}

#' @title Convert to sparse matrix
#'
#' @description
#' Convert sparse matrix  retunred from .Call to sparseMatrix.
#'
#' @param x .Call retunred list
#' @author Martin Vincent
#' @export
sparseMatrix_from_C_format <- function(x) {
	sparseMatrix(p = x[[2]], i = x[[3]], x = x[[4]], dims = x[[1]], index1 = FALSE)
}

.format_data_list <- function(data) lapply(data, function(x) {
	if( is(x, "sparseMatrix") ) {
		return( sparseMatrix_to_C_format(x) )
	}

	return(x)
})

#' @title Retur the element class of an object.
#'
#' @description
#' Return the element class of an object.
#' The object must be a matrix, vector or NULL.
#' The element class of NULL is NULL
#'
#' @param x a matrix, vector or NULL
#' @author Martin Vincent
#' @export
element_class <- function(x) {

	if( is.null(x) ) {
		return( NULL )
	} else if( is.matrix(x) ) {
		return( class(x[1,1]) )
	} else if( is(x, "Matrix") ) {
		return( class(x[1,1]) )
	} else if ( is.vector(x) ) {
		return( class(x) )
	} else if ( is.factor(x) ) {
		return( class(x) )
	} else {
		stop(paste("can not handle object of class", class(x)))
	}
}
