library(sglOptim)
library(tools)

source("units/generate_data.R")


data <- create_test_data()

# map data
X <- data$X
Y <- data$Y
W <- data$W
D <- apply(W, 2, as.integer)

### Test for erroes if x or y contains NA values
xna <- X
xna[1,1] <- NA
assertError(
  dummy <- create.sgldata(xna, y)
)

yna <- Y
yna[1,1] <- NA
assertError(
  dummy <- create.sgldata(x, yna)
)

### Test for errors if error in objective

data <- create.sgldata(X, Y)
data <- add_data(data, W, "W")
data <- add_data(data, D, "D")

assertError(
  fit <- sgl_fit(
    module_name = "linear_test_diagonal_error_w",
    PACKAGE = "sglOptim",
    data = data,
    alpha = 0,
    lambda = 0.001
  )
)

assertWarning(
  res <- sgl_test(
    module_name = "linear_test_diagonal_error_w",
    PACKAGE = "sglOptim",
    data = data,
    parameterGrouping = NULL,
    groupWeights = NULL,
    parameterWeights = NULL
  )
)

### Invalid lambda
assertError(
  fit <- sgl_cv(
    module_name =  "linear_test_diagonal_w",
    PACKAGE = "sglOptim",
    data = data,
    alpha = 0.5,
    lambda = "invalid",
    fold = 10
  )
)

### Test for error if cv.indices invalid

assertError(
  fit <- sgl_cv(
    module_name =  "linear_test_diagonal_w",
    PACKAGE = "sglOptim",
    data = data,
    alpha = 0.5,
    lambda = 0.5,
    cv.indices =  list(c(1, 5), c(5:nrow(data$data$X)))
  )
)

assertError(
  fit <- sgl_cv(
    module_name =  "linear_test_diagonal_w",
    PACKAGE = "sglOptim",
    data = data,
    alpha = 0.5,
    lambda = 0.5,
    cv.indices =  list(c(1, 5), c(6:9))
  )
)

assertError(
  fit <- sgl_cv(
    module_name =  "linear_test_diagonal_w",
    PACKAGE = "sglOptim",
    data = data,
    alpha = 0.5,
    lambda = 0.5,
    cv.indices =  list(1:nrow(data$data$X))
  )
)


### Test for error with zero data
Xz <- matrix(0, nrow = nrow(X), ncol = ncol(X))

data <- create.sgldata(Xz, Y)
data <- add_data(data, W, "W")

assertError(
  fit <- sgl_lambda_sequence(
    module_name = "linear_test_diagonal_w",
    PACKAGE = "sglOptim",
    data = data,
    alpha = 0.5,
    lambda.min = 0.05,
  )
)

### Test for erros if dim mismatch

data <- create.sgldata(X, Y)
data <- add_data(data, W, "W")
data <- add_data(data, D, "D")

# fit <- sgl_fit(
#   module_name = "linear_test_diagonal_w",
#   PACKAGE = "sglOptim",
#   data = data,
#   alpha = 1,
#   lambda = 0.05
# )

#NOTE sgl_predict


X <- X[-1, ]

assertError(
  data <- create.sgldata(X, Y)
)
