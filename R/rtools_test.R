#' @title Test internal rtools
#' @description
#' This function runs some internal tests and is not intended for users of the package.
#'
#' @author Martin Vincent
#' @export
#' @useDynLib sglOptim, .registration=TRUE
#' @importFrom utils packageVersion
#' @import Matrix
test_rtools <- function() {

  errors <- NULL

  cat("double_rtools_test \n")
  test_values <-  runif(100, -100, 100)
  stopifnot( all(sapply(test_values, function(x) x == .Call("double_rtools_test", x))))

  cat("field_double_rtools_test \n")
  test_values <- as.list(test_values)
  stopifnot(all.equal(test_values, .Call("field_double_rtools_test", test_values)))

  cat("u32_rtools_test \n")
  test_values <-  runif(0, 1e5, 100)
  stopifnot( all(sapply(test_values, function(x) x == .Call("u32_rtools_test", x))))

  cat("field_u32_rtools_test \n")
  test_values <- as.list(test_values)
  stopifnot( all.equal(test_values, .Call("field_u32_rtools_test", test_values)))

  cat("int_rtools_test \n")
  test_values <-  sample(-1e5:1e5, size = 100)
  stopifnot( all(sapply(test_values, function(x) x == .Call("int_rtools_test", x))))

  cat("field_int_rtools_test \n")
  test_values <- as.list(test_values)
  stopifnot( all.equal(test_values, .Call("field_int_rtools_test", test_values)))

  cat("vec_int_rtools_test \n")
  stopifnot( all(unlist(test_values) == .Call("vec_int_rtools_test", test_values)))

  cat("bool_rtools_test \n")
  test_values <-  c(TRUE, FALSE)
  stopifnot( all(sapply(test_values, function(x) x == .Call("bool_rtools_test", x))))

  cat("field_bool_rtools_test \n")
  test_values <- as.list(test_values)
  stopifnot( all.equal(test_values, .Call("field_bool_rtools_test", test_values)))

  cat("string_rtools_test \n")
  test_values <- sapply(1:100, function(i)
    paste(sample(c(LETTERS, letters, ".", "\\", ","), size = i, replace = TRUE), collapse = ""))
  stopifnot( all(sapply(test_values, function(x) x == .Call("string_rtools_test", x))))

  cat("field_string_rtools_test \n")
  test_values <- as.list(test_values)
  stopifnot( all.equal(test_values, .Call("field_string_rtools_test", test_values)))

  cat("vec_string_rtools_test \n")
  stopifnot( all(unlist(test_values) == .Call("vec_string_rtools_test", test_values)))

  cat("mat_double_rtools_test \n")
  test_values <- replicate(25, matrix(runif(150, -1e5, 1e5), nrow = 10, ncol = 15), simplify = FALSE)
  stopifnot( all(sapply(test_values, function(x) all(x == .Call("mat_double_rtools_test", x)))))

  cat("field_mat_double_rtools_test \n")
  stopifnot( all.equal(test_values, .Call("field_mat_double_rtools_test", test_values)))

  cat("mat_u32_rtools_test \n")
  test_values <- replicate(25, matrix(sample(0:1e5, size = 150), nrow = 10, ncol = 15), simplify = FALSE)
  stopifnot( all(sapply(test_values, function(x) all(x == .Call("mat_u32_rtools_test", x)))))

  cat("double_rtools_test \n")
  test_values <- replicate(25, matrix(sample(-1e5:1e5, size = 150), nrow = 10, ncol = 15), simplify = FALSE)
  stopifnot( all(sapply(test_values, function(x) all(x == .Call("mat_int_rtools_test", x)))))

  cat("col_double_rtools_test \n")
  test_values <- replicate(25, runif(150, -1e5, 1e5), simplify = FALSE)
  stopifnot( all(sapply(test_values, function(x) all(x == .Call("col_double_rtools_test", x)))))

  cat("field_col_double_rtools_test \n")
  stopifnot( all.equal(test_values, .Call("field_col_double_rtools_test", test_values)))


  cat("col_u32_rtools_test \n")
  test_values <- replicate(25, sample(0:1e5, size = 500), simplify = FALSE)
  stopifnot( all(sapply(test_values, function(x) all(x == .Call("col_u32_rtools_test", x)))))

  cat("field_col_u32_rtools_test \n")
  stopifnot( all.equal(test_values, .Call("field_col_u32_rtools_test", test_values)))

  cat("col_s32_rtools_test \n")
  test_values <- replicate(25, sample(-1e5:1e5, size = 500), simplify = FALSE)
  stopifnot( all(sapply(test_values, function(x) all(x == .Call("col_s32_rtools_test", x)))))

  cat("field_col_s32_rtools_test \n")
  stopifnot( all.equal(test_values, .Call("field_col_s32_rtools_test", test_values)))

  test_values <- replicate(25,
    sparseMatrix(
      i = sample(1:100, size = 50),
      j = sample(1:100, size = 50),
      x = runif(50, -1e5, 1e5)
    ),
    simplify = FALSE
  )
  test_values <- lapply(test_values, sparseMatrix_to_C_format)

  cat("sp_mat_rtools_test \n")
  stopifnot( all(sapply(test_values, function(x) all.equal(x, .Call("sp_mat_rtools_test", x)))))

  cat("field_sp_mat_rtools_test \n")
  stopifnot( all.equal(test_values, .Call("field_sp_mat_rtools_test", test_values)))

}
