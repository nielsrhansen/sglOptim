library(sglOptim)
library(tools)

source("units/generate_data.R")

set.seed(100) #  ensures consistency of tests

## Simulate from Y=XB+E, the dimension of Y is N x K, X is N x p, B is p x K

rawdata <- create_test_data(N = 15, p = 20, K = 5)
data <- create.sgldata(rawdata$X, rawdata$Y)
data <- add_data(data, rawdata$W, "W")


assertWarning(
  fit <- sgl_cv(
    module_name =  "linear_test_diagonal_w",
    PACKAGE = "sglOptim",
    data = data,
    alpha = 0.5,
    lambda = 0.1,
    fold = 10,
    max.threads = 2
))
