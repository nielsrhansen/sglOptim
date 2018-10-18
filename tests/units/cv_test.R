cv_test <- function(data, values, consistency, i, j) {

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

  set.seed(100) #  ensures consistency of tests

  fit <- sgl_cv(
    module_name = callsym,
    PACKAGE = "sglOptim",
    data = data,
    parameterGrouping = values$grouping[[i]],
    groupWeights = values$groupWeights[[i]],
    parameterWeights = values$parameterWeights[[i]],
    alpha = values$alpha[i],
    lambda = values$lambda.min[i],
    d = values$d[i],
    compute_lambda = TRUE,
    fold = values$fold[i],
    sampleGroups = values$sampleGroups[[i]],
    cv.indices = values$cv.indices[[i]],
    responses = values$responses[[i]]
  )

  if(nmod(fit) != values$d[i]) stop()

# Check dimensions and names
if( ! all.equal(dimnames(Y), dimnames(fit$Y.true)) ) stop()

  if(is.null(values$responses[[i]])) {

    stopifnot(length(fit$responses[[1]][[1]]$link) == data$response_dimension)

    if( ! is.null(names(fit$responses[[1]][[1]]$link)) ) {
      stopifnot( ! is.null(data$response_names) )
      stopifnot(names(fit$responses[[1]][[1]]$link) == data$response_names)
    } else {
      stopifnot( is.null(data$response_names) )
    }
  } else {

    if(is.list(fit$responses$link)) {

      r <- fit$responses$link[[1]]

      stopifnot(all(dim(r) == c(data$n_samples, data$response_dimension)))

      if( ! is.null(dimnames(r)) ) {

        stopifnot( ! is.null(data$response_names) &&
          colnames(r) == data$response_names
        )

        stopifnot( ! is.null(data$sample_names) &&
          rownames(r) == data$sample_names
        )

      } else {
        stopifnot( is.null(data$response_names) )
        stopifnot( is.null(data$samnpl_names) )
      }

    } else {

      r <- fit$responses$link

      stopifnot(all(dim(r) == c(data$n_samples, values$d[i])))

      if( ! is.null(dimnames(r)) ) {

        stopifnot( ! is.null(data$response_names) &&
          all(colnames(r) == data$response_names)
        )

        stopifnot( ! is.null(data$sample_names) &&
          rownames(r) == data$sample_names
        )

      } else {
        stopifnot( is.null(data$response_names) )
        stopifnot( is.null(data$samnpl_names) )
      }

    }



  }

  # features and parameters
  features_stat(fit)
  parameters_stat(fit)

  best_model(fit, "sgl")

  # print
  sgl_print(fit)

  # Err
  err <- Err(fit)

  if( ! is.null(values$responses[[i]]) ) {

    # Test compute_error with transposed response

    fit$responses <- transpose_response_elements(fit$responses)

    loss <- function(x,y) mean(sapply(1:length(x),
      function(i) sqrt(mean((x[[i]] - y[[i]])^2)))
    )

    err2 <- compute_error(
      x = fit,
      response_name = "link",
      true_response = fit$Y.true,
      loss = loss,
      transposed_response = TRUE
    )

    stopifnot( all(abs(err2 - err) < 1e-5) )
  }

  return( err )
}


check_cv_consistency <- function(consistency_list) {

  # consistency beta
  err_ref <- consistency_list[[1]]

  e <- sapply(consistency_list, function(err) max(abs(err - err_ref)))

  if(max(e) > 1e-3) stop("cv args consistency test failed")
}
