subsampling_test_1 <- function(data, values, consistency, i, j) {

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

  if( values$test_train[i] == "A" ) {

    test <- replicate(2, 1:(data$n_samples/2), simplify = FALSE)
    train <- lapply(test, function(s) (1:data$n_samples)[-s])

  } else if( values$test_train[i] == "B" ) {

    test <- as.list(1:data$n_samples) # Test single tests
    train <- replicate(data$n_samples, 1:data$n_samples, simplify = FALSE)

  } else if( values$test_train[i] == "C" ) {

    test <- as.list(data$sample_names) # Test single tests
    train <- replicate(data$n_samples, 1:data$n_samples, simplify = FALSE)

  } else {
    stop("unkown test_train")
  }

  fit <- sgl_subsampling(
    module_name = module,
    PACKAGE = "sglOptim",
    data = data,
    parameterGrouping = values$grouping[[i]],
    groupWeights = values$groupWeights[[i]],
    parameterWeights = values$parameterWeights[[i]],
    alpha = values$alpha[i],
    lambda = values$lambda.min[i],
    d = values$d[i],
    compute_lambda = TRUE,
    training = train,
    test = test,
    responses = values$responses[[i]],
    use_parallel = values$use_parallel[i]
  )

  if(nmod(fit) != values$d[i]) stop()

  #NOTE dimension and name check

  # features and parameters
  features_stat(fit)
  parameters_stat(fit)


  best_model(fit, "sgl")

  # print
  sgl_print(fit)

  return( NULL ) #NOTE
}

check_subsampling_consistency <- function(consistency_list) {
  #NOTE
}
