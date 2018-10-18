fit_test <- function(data, values, consistency, i, j) {

  print(values[i,])
  print(consistency[j,])

  # map data
  X <- data$X
  Y <- data$Y
  W <- data$W

  # create data
  data <- create.sgldata(X, Y, sparseX = consistency$sparseX[j], sparseY = consistency$sparseY[j])
  data <- add_data(data, W, "W")

  module <- paste(
    consistency$objective[j],
    if(data$sparseX) "_spx" else "",
    if(data$sparseY) "_spy" else "",
    sep = ""
  )

  fit <- sgl_fit(
    module_name = module,
    PACKAGE = "sglOptim",
    data = data,
    parameterGrouping = values$grouping[[i]],
    groupWeights = values$groupWeights[[i]],
    parameterWeights = values$parameterWeights[[i]],
    alpha = values$alpha[i],
    lambda = values$lambda.min[i],
    d = values$d[i]
  )

  if(nmod(fit) != values$d[i]) stop()

  # models
  beta_list <- models(fit)
  if(length(beta_list) != values$d[i]) stop()
  if( ! all(sapply(beta_list, function(beta) all(dim(beta) == c(ncol(Y), ncol(X)))))) stop()

  # check colnames and rownames of models
  if( ! is.null(dimnames(Y)) ) {
    if( ! all.equal(dimnames(Y), dimnames(fit$Y.true))) stop()
  }

  if( is.null(values$parameterWeights[[i]]) && (! is.null(colnames(Y)) || ! is.null(colnames(X))) ) {
    if( ! all.equal(dimnames(beta_list[[1]]), list(colnames(Y), colnames(X))) ) stop()
  }

  # coef
  if(length(coef(fit)) != values$d[i]) stop()

  # print
  sgl_print(fit)

  res <- sgl_predict(
    module_name = module,
    PACKAGE = "sglOptim",
    object = fit,
    data = data,
    responses = values$response[[i]]
  )

  sgl_print(res)

  if( is.null(values$response[[i]]) ) {

    assertError(
      dummy <- transpose_response_elements(res$response)
    )

  }  else {

    dummy <- transpose_response_elements(res$responses$link)
    dummy <- transpose_response_elements(res$responses)

  }

  # return beta matrix for args consistency test
  return( beta_list )
}

check_fit_consistency <- function(consistency_list) {

  # consistency beta
  beta_ref <- consistency_list[[1]]

  e <- sapply(consistency_list, function(beta) max(sapply(1:length(beta), function(i)
    mean(abs(beta[[i]]-beta_ref[[i]]))))
  )

  if(max(e) > 1e-2) stop("test failed")
}
