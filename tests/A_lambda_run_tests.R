# To run all tests on linux do in shell: NOT_CRAN=true; export NOT_CRAN

library(sglOptim)

source("units/run_tests.R")
source("units/generate_data.R")
source("units/lambda_test.R")

# warnings = errors
options(warn=2)
set.seed(100) #  ensures consistency of tests

# Run the tests

## create data
data <- create_test_data()

## Possible args values
values <- expand.grid(
  grouping = list(
    NULL,
    factor(1:ncol(data$X) %% 7)
    ),
  groupWeights = list(NULL),
  parameterWeights = list(NULL),
  alpha = c(0, 0.5, 1),
  d = 100,
  lambda.min = 0.8,
  lambda.min.rel = TRUE
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

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = lambda_test,
  check_consistency = check_lambda_consistency
)

data$Y <- data$Y[, 1, drop = FALSE]
data$W <- data$W[, 1, drop = FALSE]

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = lambda_test,
  check_consistency = check_lambda_consistency
)

data$X <- Matrix(data$X, sparse = TRUE)

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = lambda_test,
  check_consistency = check_lambda_consistency
)
