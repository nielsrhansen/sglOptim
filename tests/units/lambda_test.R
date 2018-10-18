lambda_test <- function(data, values, consistency, i, j) {

  # map data
  X <- data$X
  Y <- data$Y
  W <- data$W

  # create data
  data <- create.sgldata(X, Y, sparseX = consistency$sparseX[j], sparseY = consistency$sparseY[j])
  data <- add_data(data, W, "W")

  callsym <- paste(
    consistency$objective[j],
    if(data$sparseX) "_spx" else "",
    if(data$sparseY) "_spy" else "",
    sep = ""
  )

  lambda <- sgl_lambda_sequence(
    module_name = callsym,
    PACKAGE = "sglOptim",
    data = data,
    parameterGrouping = values$grouping[[i]],
    groupWeights = values$groupWeights[[i]],
    parameterWeights = values$parameterWeights[[i]],
    alpha = values$alpha[i],
    d = values$d[i],
    lambda.min = values$lambda.min[i],
    lambda.min.rel = values$lambda.min.rel[i]
  )

  #NOTE test lambda is decreasing and positive

  return( lambda )
}

check_lambda_consistency <- function(consistency_list) {

  lambda_ref <- consistency_list[[1]]
  e <- sapply(consistency_list, function(lambda) max(abs(lambda-lambda_ref)))

  if(max(e) > 1e-7) stop("test failed")
}
