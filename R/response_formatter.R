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

.format_response <- function(response_list, name) {

  if( is.null(name) ) {
    stop("name can not be NULL")
  }

  type <- attr(response_list, "type")

  if( ! is.null(type) && type[1] == "subsample") {

    res <-lapply( 1:length(response_list),
      function(i) .format_response(response_list[[i]], name)
    )

    attr(res, "type") <- c("subsample", attr(res[[1]], "type", exact = TRUE))

    return( res )
  }

  if( ! name %in% names(response_list[[1]][[1]]) ) {
    stop(paste("response '", name, "' not found", sep=""))
  }

  elem1 <- response_list[[1]][[1]][[name]]

  if(is.vector(elem1) && length(elem1) == 1) {

    res <- t(sapply(response_list, function(x) sapply(x, function(y) y[[name]])))
    attr(res, "type") <- c("response")

    return ( res )
  }

  if(is.vector(elem1)) {

    res <- lapply(1:length(response_list[[1]]), function(i) t(sapply(response_list, function(x) x[[i]][[name]])))
    attr(res, "type") <- c("model", "response")

    return ( res )
  }

  res <- lapply(response_list, function(x) lapply(x, function(y) y[[name]]))
  attr(res, "type") <- c("sample", "model", "response")

  return ( res )
}

.set_response_names <- function(response_list, data) {

  rd <- data$response_dimension
	rn <- data$response_names

	if( ! is.null(rd) && ! is.null(rn)) {

		elems <- response_list[[1]][[1]]
    elems_names <- names(response_list[[1]][[1]])

    for(i in 1:length(elems)) {
			if( is.vector(elems[[i]]) && length(elems[[i]]) == rd ) {
				response_list <- lapply(response_list, function(a1)
					lapply(a1, function(a2) {
						names(a2[[i]]) <- rn
						return( a2 )
					})
				)
			} else if( is.matrix(elems[[i]]) && dim(elems[[i]]) == c(rd, rd) ) {
				response_list <- lapply(response_list, function(a1)
					lapply(a1, function(a2) {
						dimnames(a2[[i]]) <- list(rn, rn)
						return( a2 )
				 })
				)
			}
		}
	}

  return( response_list )
}

#' @title Transpose response elements
#'
#' @description Transpose response elements in a response list and sub lists
#' @param x response list or matrix
#' @return response list with all matrices transposed
#' @author Martin Vincent
#' @export
transpose_response_elements <- function(x) {

  type <- attr(x, "type")

  if( is.matrix(x) ) {

    x <- t(x)

    attr(x, "type") <- type
    return(x)
  }

  if( is.list(x) && is.matrix(x[[1]]) ) {

    x <- lapply(x, t)

    attr(x, "type") <- type
    return(x)
  }

  if( is.list(x) && is.list(x[[1]]) && is.matrix(x[[1]][[1]]) ) {

    x <- lapply(x, function(y) {

      z <- lapply(y, t)

      attr(z, "type") <- attr(y, "type")
      return(z)

    })

    attr(x, "type") <- type
    return(x)
  }

  stop("response list too deep or has no matrices")
}
