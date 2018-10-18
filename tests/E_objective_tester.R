library(sglOptim)

# warnings = errors
options(warn=2)

## Simulate from Y=XB+E, the dimension of Y is N x K, X is N x p, B is p x K

N <- 15 #number of samples
p <- 20 #number of features
K <- 5  #number of groups

B <- matrix(sample(c(rep(1,p*K*0.1),rep(0, p*K-as.integer(p*K*0.1)))),nrow=p,ncol=K)
X <- matrix(rnorm(N*p,1,1),nrow=N,ncol=p)
Y <- X%*%B+matrix(rnorm(N*K,0,1), nrow = N, ncol = K)
W <- matrix(1/N, nrow = N, ncol = K)

covariateGrouping <- factor(1:ncol(X))
groupWeights <- sqrt(ncol(Y)*table(covariateGrouping))
parameterWeights <-  matrix(1, nrow = ncol(Y), ncol = ncol(X))

# create data
data <- create.sgldata(X, Y)
data <- add_data(data, W, "W")

res <- sgl_test(
  module_name = "linear_test_diagonal_w",
  PACKAGE =   "sglOptim",
  data = data,
  parameterGrouping =  covariateGrouping,
  groupWeights = groupWeights,
  parameterWeights = parameterWeights
)

if(res != 0) stop()

#NOTE test objective with error
