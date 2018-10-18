library(sglOptim)

source("units/run_tests.R")
source("units/generate_data.R")
source("units/subsampling_test_1.R")

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
  test_train = c("A", "B"),
  responses = list("link"),
  use_parallel = TRUE
)

## consistency args values
consistency <- expand.grid(
  sparseX = c(TRUE, FALSE),
  sparseY = c(TRUE, FALSE),
  objective = c("linear_test_diagonal_w", "linear_test_identity")
)

cl <- makeCluster(2)
registerDoParallel(cl)

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = subsampling_test_1,
  check_consistency = check_subsampling_consistency
)

data$Y <- data$Y[, 1, drop = FALSE]
data$W <- data$W[, 1, drop = FALSE]

run_tests(
  data = data,
  args_values = values,
  args_consistency = consistency,
  test = subsampling_test_1,
  check_consistency = check_subsampling_consistency,
)

stopCluster(cl)
