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
  grouping = list(
    NULL,
    factor(1:ncol(data$X) %% 7)
  ),
  groupWeights = list(NULL),
  parameterWeights = list(NULL),
  alpha = c(0, 0.5, 1),
  d = 100,
  fold = c(2, nrow(data$X)),
  sampleGroups = list(
    NULL,
    factor(1:nrow(data$X) %% 3)
  ),
  cv.indices = list(
    NULL,
    list(c(1:5), c(6:nrow(data$X)))
  ),
  responses = list(
    "link",
    NULL
  ),
  lambda.min = 0.8,
  lambda.min.rel = TRUE
)

## consistency args values
consistency <- expand.grid(
  sparseX = c(TRUE, FALSE),
  sparseY = c(TRUE, FALSE),
  objective = "linear_test_identity"
)

not_on_cran(
  run_tests(
    data = data,
    args_values = values,
    args_consistency = consistency,
    test = cv_test,
    check_consistency = check_cv_consistency
))

data$Y <- data$Y[, 1, drop = FALSE]
data$W <- data$W[, 1, drop = FALSE]

not_on_cran(
  run_tests(
    data = data,
    args_values = values,
    args_consistency = consistency,
    test = cv_test,
    check_consistency = check_cv_consistency
))
