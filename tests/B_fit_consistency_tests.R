library(sglOptim)

# warnings = errors
options(warn=2)

set.seed(100) #  ensures consistency of tests

## Simulate from Y=XB+E, the dimension of Y is N x K, X is N x p, B is p x K

N <- 15 #number of samples
p <- 20 #number of features
K <- 5  #number of groups

B <- matrix(sample(c(rep(1,p*K*0.1),rep(0, p*K-as.integer(p*K*0.1)))), nrow=p, ncol=K)
X <- matrix(rnorm(N*p,1,1), nrow=N, ncol=p)
Y <- X%*%B+matrix(rnorm(N*K,0,1), nrow = N, ncol = K)
W <- matrix(1/N, nrow = N, ncol = K)

data <- create.sgldata(X, Y)
data <- add_data(data, W, "W")

fit <- sgl_fit(
  module_name = "linear_test_diagonal_w",
  PACKAGE = "sglOptim",
  data = data,
  alpha = 0.5,
  lambda = 0.05,
)

# Some performance tests
err <- Err(fit, X)
if(min(err) > 0.9) stop()
if(max(err) < 2.7) stop()
if(any(diff(err) > 0)) stop()

feat <- features_stat(fit)
para <- parameters_stat(fit)

if(max(feat) > 15) stop()
if(max(feat) < 10) stop()
if(min(feat) > 1) stop()
if(max(para) > 45 ) stop()
if(max(para) < 40 ) stop()
if(min(para) > 1 ) stop()


fit <- sgl_fit(
  module_name = "linear_test_diagonal_w",
  PACKAGE = "sglOptim",
  data = data,
  alpha = 0,
  lambda = 0.05,
)

# Some performance tests
err <- Err(fit, X)
if(min(err) > 0.9) stop()
if(max(err) < 2.7) stop()
if(any(diff(err) > 0)) stop()

feat <- features_stat(fit)
para <- parameters_stat(fit)

if(max(feat) > 16) stop()
if(max(feat) < 10) stop()
if(min(feat) > 1) stop()
if(max(para) > 75 ) stop()
if(max(para) < 65 ) stop()
if(min(para) > 5 ) stop()


fit <- sgl_fit(
  module_name = "linear_test_diagonal_w",
  PACKAGE = "sglOptim",
  data = data,
  alpha = 1,
  lambda = 0.05,
)

# Some performance tests
err <- Err(fit, X)
if(min(err) > 1) stop()
if(max(err) < 2.7) stop()
if(any(diff(err) > 0)) stop()

feat <- features_stat(fit)
para <- parameters_stat(fit)

if(max(feat) > 16) stop()
if(max(feat) < 10) stop()
if(min(feat) > 1) stop()
if(max(para) > 30 ) stop()
if(max(para) < 25 ) stop()
if(min(para) > 1 ) stop()
