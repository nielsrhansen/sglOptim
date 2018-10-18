library(sglOptim)
library(tools)

source("units/run_tests.R")
source("units/generate_data.R")
source("units/fit_test.R")

# warnings = errors
options(warn=2)
set.seed(100) #  ensures consistency of tests

# Run the tests

## create data
data <- create_test_data()

p <- ncol(data$X)
q <- ncol(data$Y)

pW <- matrix(runif(p*q), nrow = q, ncol = p)

## Possible args values
values <- expand.grid(
  grouping = list(NULL),
  groupWeights = list(
    NULL,
    c(0, rep(1, p-1))
  ),
  parameterWeights = list(
    pW,
    NULL
  ),
  alpha = c(0, 0.5, 1),
  d = 50,
  lambda.min = 0.8,
  lambda.min.rel = TRUE,
  response = list(
    NULL,
    "link"
  )
)

## consistency args values
consistency <- expand.grid(
  sparseX = c(TRUE, FALSE),
  sparseY = c(TRUE, FALSE),
  objective = c(
    "linear_test_diagonal_w",
    "linear_test_identity",
    "linear_test_full",
    "linear_test_block_diagonal"
  )
)

not_on_cran(
  run_tests(
    data = data,
    args_values = values,
    args_consistency = consistency,
    test = fit_test,
    check_consistency = check_fit_consistency
))
