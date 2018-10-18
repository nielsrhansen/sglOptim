library(sglOptim)
source("units/generate_data.R")

# warnings = errors
options(warn=2)

set.seed(100) #  ensures consistency of tests

## Simulate from Y=XB+E, the dimension of Y is N x K, X is N x p, B is p x K

rawdata <- create_test_data(N = 15, p = 20, K = 5)
data <- create.sgldata(rawdata$X, rawdata$Y)
data <- add_data(data, rawdata$W, "W")

fit <- sgl_cv(
  module_name =  "linear_test_diagonal_w",
  PACKAGE = "sglOptim",
  data = data,
  alpha = 0.5,
  lambda = 0.1,
  fold = 10
)

 sgl_print(fit)

 # Some performance tests
 err <- Err(fit)
 if(min(err) > 1.9) stop()
 if(max(err) < 2.7) stop()
 if(any(diff(err) > 1e-3)) stop()

if(best_model(fit, "sgl") != 100) stop()

 feat <- colMeans(features_stat(fit))
 para <- colMeans(parameters_stat(fit))

 if(max(feat) > 11) stop()
 if(max(feat) < 8) stop()
 if(min(feat) > 2) stop()
 if(max(para) > 35 ) stop()
 if(max(para) < 25 ) stop()
 if(min(para) > 2 ) stop()


 fit <- sgl_cv(
   module_name =  "linear_test_diagonal_w",
   PACKAGE = "sglOptim",
   data = data,
   alpha = 0,
   lambda = 0.1,
   fold = 10
 )

  sgl_print(fit)

  # Some performance tests
  err <- Err(fit)
  if(min(err) > 1.5) stop()
  if(max(err) < 2.7) stop()
  if(any(diff(err) > 1e-3)) stop()

 if(best_model(fit, "sgl") != 100) stop()

  feat <- colMeans(features_stat(fit))
  para <- colMeans(parameters_stat(fit))

  if(max(feat) > 11) stop()
  if(max(feat) < 8) stop()
  if(min(feat) > 2) stop()
  if(max(para) > 55 ) stop()
  if(max(para) < 45 ) stop()
  if(min(para) > 6 ) stop()


 fit <- sgl_cv(
   module_name =  "linear_test_diagonal_w",
   PACKAGE = "sglOptim",
   data = data,
   alpha = 1,
   lambda = 0.1,
   fold = 10
 )

  sgl_print(fit)

  # Some performance tests
  err <- Err(fit)
  if(min(err) > 1.7) stop()
  if(max(err) < 2.7) stop()
  if(any(diff(err) > 1e-3)) stop()

 if(best_model(fit, "sgl") != 100) stop()

  feat <- colMeans(features_stat(fit))
  para <- colMeans(parameters_stat(fit))

  if(max(feat) > 12) stop()
  if(max(feat) < 8) stop()
  if(min(feat) > 2) stop()
  if(max(para) > 25 ) stop()
  if(max(para) < 15 ) stop()
  if(min(para) > 2 ) stop()
