library(sglOptim)

source("units/run_tests.R")
source("units/generate_data.R")
source("units/subsampling_test_1.R")
source("units/subsampling_test_2.R")

# warnings = errors
options(warn=2)
set.seed(100) #  ensures consistency of tests

# Run the tests

## create data
data <- create_test_data()

## Possible args values
values <- expand.grid(
  grouping = list(NULL),
  groupWeights = list(NULL),
  parameterWeights = list(NULL),
  alpha = c(0, 0.5, 1),
  d = 100,
  lambda.min = 0.8,
  lambda.min.rel = TRUE,
  test_train = c("C", "A", "B"),
  responses = list(
    NULL,
    "link"
  ),
  use_parallel = FALSE
)

## consistency args values
consistency <- expand.grid(
  sparseX = c(TRUE, FALSE),
  sparseY = c(TRUE, FALSE),
  objective =  "linear_test_diagonal_w"
)

not_on_cran(
  run_tests(
    data = data,
    args_values = values,
    args_consistency = consistency,
    test = subsampling_test_1,
    check_consistency = check_subsampling_consistency
))

data$Y <- data$Y[, 1, drop = FALSE]
data$W <- data$W[,1, drop = FALSE]

not_on_cran(
  run_tests(
    data = data,
    args_values = values,
    args_consistency = consistency,
    test = subsampling_test_1,
    check_consistency = check_subsampling_consistency
))


## consistency args values
consistency <- expand.grid(
  sparseX = FALSE,
  sparseY = FALSE,
  objective = "linear_test_diagonal_w"
)

not_on_cran(
  run_tests(
    data = data,
    args_values = values,
    args_consistency = consistency,
    test = subsampling_test_2,
    check_consistency = check_subsampling_consistency
))
