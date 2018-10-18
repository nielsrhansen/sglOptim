#' sglOptim: Generic Sparse Group Lasso Solver
#'
#' Fast generic solver for sparse group lasso optimization problems.
#' The loss (objective) function must be defined in a C++ module.
#' The optimization problem is solved using a coordinate gradient descent algorithm.
#' Convergence of the algorithm is established (see reference) and the algorithm is applicable to a broad class of loss functions.
#' Use of parallel computing for cross validation and subsampling is supported through the 'foreach' and 'doParallel' packages.
#' Development version is on GitHub, please report package issues on GitHub.
#'
#' Computes a sequence of minimizers (one for each lambda given in the \code{lambda} argument) of
#' \deqn{\mathrm{loss}(\beta) + \lambda \left( (1-\alpha) \sum_{J=1}^m \gamma_J \|\beta^{(J)}\|_2 + \alpha \sum_{i=1}^{n} \xi_i |\beta_i| \right)}
#' where \eqn{\mathrm{loss}} is the loss/objective function specified by \code{module_name}.
#' The parameters are organized in the parameter matrix \eqn{\beta} with dimension \eqn{q\times p}.
#' The vector \eqn{\beta^{(J)}} denotes the \eqn{J} parameter group.
#' The group weights \eqn{\gamma \in [0,\infty)^m} and the parameter weights \eqn{\xi = (\xi^{(1)},\dots, \xi^{(m)}) \in [0,\infty)^n}
#' with \eqn{\xi^{(1)}\in [0,\infty)^{n_1},\dots, \xi^{(m)} \in [0,\infty)^{n_m}}.
#'
#' The package includes generic functions for:
#' \itemize{
#' \item Fitting models using sparse group lasso, that is computing the minimizers of the above equation.
#' \item Cross validation using parallel computing.
#' \item Generic subsampling using parallel computing.
#' \item Applying the fitted models on new data and predicting responses.
#' \item Computing lambda sequences.
#' \item Navigating the models and computing error rates.
#'}
#'
#' @docType package
#' @author Martin Vincent \email{martin.vincent.dk@gmail.com}
#' @name sglOptim
#' @importFrom tools assertError
#' @importFrom tools assertWarning
#' @importFrom stats runif
NULL
