#### linear_test_diagonal_w ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_diagonal_w_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}

#### linear_test_identity ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_identity_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_identity_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_identity_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}

#### linear_test_full ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_full_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_full_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_full_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}




#### linear_test_block_diagonal ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_block_diagonal_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}


#### linear_test_diagonal_w_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_diagonal_w_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}

#### linear_test_identity_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_identity_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_identity_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_identity_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_full_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_full_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_full_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_full_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_block_diagonal_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_block_diagonal_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_diagonal_w_spx ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_diagonal_w_spx_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_identity_spx ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_identity_spx_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}

#### linear_test_full_spx ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_full_spx_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_spx_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_full_spx_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_spx_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_full_spx_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_block_diagonal_spx ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_block_diagonal_spx_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_diagonal_w_spx_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_diagonal_w_spx_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_w_spx_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_diagonal_w_spx_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_identity_spx_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_identity_spx_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_identity_spx_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_identity_spx_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_full_spx_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_full_spx_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_spx_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_full_spx_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_full_spx_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_full_spx_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_full_spx_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_block_diagonal_spx_spy ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_spy_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_spy_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_spy_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_spy_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_spy_sgl_predict_R <- function(data, beta) {
  .Call(linear_test_block_diagonal_spx_spy_sgl_predict, PACKAGE = "sglOptim", data, beta)
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_spy_sgl_subsampling_R <- function(
  data,
  test_data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_spy_sgl_subsampling, PACKAGE = "sglOptim",
        data,
        test_data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_block_diagonal_spx_spy_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_block_diagonal_spx_spy_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
#### linear_test_diagonal_error_w ####

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_error_w_sgl_lambda_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  d,
  lambda.min,
  lambda.min.rel,
  algorithm.config) {
  
  .Call(linear_test_diagonal_error_w_sgl_lambda, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        d,
        lambda.min,
        lambda.min.rel,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_error_w_sgl_fit_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  alpha,
  lambda,
  idx,
  algorithm.config) {
  
  .Call(linear_test_diagonal_error_w_sgl_fit, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        alpha,
        lambda,
        idx,
        algorithm.config
  )
}

#' C interface
#'
#' @keywords internal
#' @export
linear_test_diagonal_error_w_sgl_test_R <- function(
  data,
  block_dim,
  groupWeights,
  parameterWeights,
  algorithm.config) {
  
  .Call(linear_test_diagonal_error_w_sgl_test, PACKAGE = "sglOptim",
        data,
        block_dim,
        groupWeights,
        parameterWeights,
        algorithm.config
  )
}
