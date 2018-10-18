run_tests <- function(
  data,
  args_values,
  args_consistency,
  test,
  check_consistency,
  i = NULL,
  j = NULL) {

  if( is.null(i) ) {
   i_idx <- 1:nrow(args_values)
  } else {
   i_idx <- i
  }

  if( is.null(j) ) {
   j_idx <- 1:nrow(args_consistency)
  } else {
   j_idx <- j
  }

  for(i in i_idx) {

    consistency_list <- NULL

    for(j in j_idx) {

      print(paste("doing test", i,j))

      consistency_list[[length(consistency_list) + 1]] <-
        test(data, args_values, args_consistency, i, j)
    }

    check_consistency(consistency_list)

  }
}

not_on_cran <- function(fun) {

  print(Sys.getenv("NOT_CRAN"))

  if(Sys.getenv("NOT_CRAN") != "true") {
    message("On CRAN skipping test")
    return( NULL )
  }

  fun
}
