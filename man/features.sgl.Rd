% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/sgl_navigate.R
\name{features.sgl}
\alias{features.sgl}
\title{Extracting nonzero features}
\usage{
\method{features}{sgl}(object, ...)
}
\arguments{
\item{object}{a sgl object}

\item{...}{ignored}
}
\value{
a list of vectors containing the nonzero features (that is nonzero columns of the \eqn{beta} matrices)
}
\description{
Extract the nonzero features of the fitted models
}
\author{
Martin Vincent
}
