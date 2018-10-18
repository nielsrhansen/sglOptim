
create_test_data <- function(N = 15, p = 15 , K = 5, setnames = TRUE) {

  ## Simulate from Y=XB+E, the dimension of Y is N x K, X is N x p, B is p x K

  B <- matrix(sample(c(rep(1,p*K*0.1),rep(0, p*K-as.integer(p*K*0.1)))),nrow=p,ncol=K)
  X <- matrix(rnorm(N*p,1,1),nrow=N,ncol=p)
  Y <- X%*%B+matrix(rnorm(N*K,0,1), nrow = N, ncol = K)
  W <- matrix(1/N, nrow = N, ncol = K)

  if(setnames) {
    rownames(X) <- paste("sample", 1:N)
    colnames(X) <- paste("feature", 1:p)
    rownames(Y) <- paste("sample", 1:N)
    colnames(Y) <- paste("model", 1:K)
    # do not set names on W
  }

  data <- list()
  data$X <- X
  data$Y <- Y
  data$W <- W

  return( data )
}
