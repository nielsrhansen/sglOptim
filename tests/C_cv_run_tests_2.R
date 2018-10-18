library(sglOptim)

source("units/run_tests.R")
source("units/generate_data.R")
source("units/cv_test.R")

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
  fold = 2,
  sampleGroups = list(NULL),
  cv.indices = list(NULL),
  responses = list("link"),
  lambda.min = 0.8,
  lambda.min.rel = TRUE
)

## consistency args values
consistency <- expand.grid(
  sparseX = FALSE,
  sparseY = FALSE,
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
    test = cv_test,
    check_consistency = check_cv_consistency
))
