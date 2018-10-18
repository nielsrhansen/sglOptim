# SMALL CRAN TEST

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

## Possible args values
values <- expand.grid(
  grouping = list(
    NULL,
    factor(1:ncol(data$X) %% 7)
  ),
  groupWeights = list(NULL),
  parameterWeights = list(NULL),
  alpha = c(0, 0.5, 1),
  d = 50,
  lambda.min = 0.9,
  lambda.min.rel = TRUE,
  response = list(
    NULL,
    "link"
  )
)

## consistency args values
consistency <- expand.grid(
  sparseX = c(FALSE, TRUE),
  sparseY = c(FALSE, TRUE),
  objective = "linear_test_identity"
)

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = fit_test,
  check_consistency = check_fit_consistency
)

data$Y <- data$Y[, 1, drop = FALSE]
data$W <- data$W[,1, drop = FALSE]

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = fit_test,
  check_consistency = check_fit_consistency
)

dimnames(data$X) <- NULL
dimnames(data$Y) <- NULL

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = fit_test,
  check_consistency = check_fit_consistency
)
