% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/sgl_navigate.R
\name{parameters.sgl}
\alias{parameters.sgl}
\title{Extracting nonzero parameters}
\usage{
\method{parameters}{sgl}(object, ...)
}
\arguments{
\item{object}{a sgl object}

\item{...}{ignored}
}
\value{
a list of vectors containing the nonzero parameters (that is nonzero entries of the \eqn{beta} matrices)
}
\description{
Extract the nonzero parameters in each model.
Only the parameters of nonzero features (columns of the \eqn{beta} matrices) are returned.
}
\author{
Martin Vincent
}
