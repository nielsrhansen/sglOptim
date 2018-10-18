// Registration macro
#ifndef RTOOLS_TEST
#define RTOOLS_TEST(TYPE) CALL_METHOD(rtools_test, TYPE, 1)
#endif


extern "C" {
  SEXP R_FUN_NAME(rtools_test, double)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, int)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, char)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, u32)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, bool)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, string)(SEXP exp);

  SEXP R_FUN_NAME(rtools_test, mat_double)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, mat_u32)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, mat_int)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, col_double)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, col_u32)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, col_s32)(SEXP exp);

  SEXP R_FUN_NAME(rtools_test, vec_int)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, vec_string)(SEXP exp);

  SEXP R_FUN_NAME(rtools_test, field_sp_mat)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_double)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_int)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_char)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_u32)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_bool)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_string)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_mat_double)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_col_double)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_col_u32)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_col_s32)(SEXP exp);
  SEXP R_FUN_NAME(rtools_test, field_sp_mat)(SEXP exp);
}

template<typename T>
SEXP rtools_test(SEXP exp) {
  const T x = get_value<T>(exp);
  return rObject(x);
}

// Base type tests

SEXP R_FUN_NAME(rtools_test, double)(SEXP exp) {
  return rtools_test<double>(exp);
}

SEXP R_FUN_NAME(rtools_test, int)(SEXP exp) {
  return rtools_test<int>(exp);
}

SEXP R_FUN_NAME(rtools_test, u32)(SEXP exp) {
  return rtools_test<arma::u32>(exp);
}

SEXP R_FUN_NAME(rtools_test, bool)(SEXP exp) {
  return rtools_test<bool>(exp);
}

SEXP R_FUN_NAME(rtools_test, string)(SEXP exp) {
  return rtools_test<std::string>(exp);
}

//Base arma type tests

SEXP R_FUN_NAME(rtools_test, mat_double)(SEXP exp) {
  return rtools_test<arma::Mat<double> >(exp);
}

SEXP R_FUN_NAME(rtools_test, mat_u32)(SEXP exp) {
  return rtools_test<arma::umat>(exp);
}

SEXP R_FUN_NAME(rtools_test, mat_int)(SEXP exp) {
  return rtools_test<arma::imat>(exp);
}

SEXP R_FUN_NAME(rtools_test, col_double)(SEXP exp) {
  return rtools_test<arma::Col<double> >(exp);
}

SEXP R_FUN_NAME(rtools_test, col_u32)(SEXP exp) {
  return rtools_test<arma::Col<arma::u32> >(exp);
}

SEXP R_FUN_NAME(rtools_test, col_s32)(SEXP exp) {
  return rtools_test<arma::Col<arma::s32> >(exp);
}

SEXP R_FUN_NAME(rtools_test, sp_mat)(SEXP exp) {
  return rtools_test<arma::sp_mat>(exp);
}

// TEST vector

template<typename T>
SEXP rtools_test_vector(SEXP exp) {
  const std::vector<T> x = get_vector<T>(exp);
  return rObject(x);
}

SEXP R_FUN_NAME(rtools_test, vec_int)(SEXP exp) {
  return rtools_test_vector<int>(exp);
}

SEXP R_FUN_NAME(rtools_test, vec_string)(SEXP exp) {
  return rtools_test_vector<std::string>(exp);
}

// TEST fields
template<typename T>
SEXP rtools_test_field(SEXP exp) {
  const arma::field<T> x = get_field<T>(exp);
  return rObject(x);
}

SEXP R_FUN_NAME(rtools_test, field_double)(SEXP exp) {
  return rtools_test_field<double>(exp);
}

SEXP R_FUN_NAME(rtools_test, field_int)(SEXP exp) {
  return rtools_test_field<int>(exp);
}

SEXP R_FUN_NAME(rtools_test, field_u32)(SEXP exp) {
  return rtools_test_field<arma::u32>(exp);
}

SEXP R_FUN_NAME(rtools_test, field_bool)(SEXP exp) {
  return rtools_test_field<bool>(exp);
}

SEXP R_FUN_NAME(rtools_test, field_string)(SEXP exp) {
  return rtools_test_field<std::string>(exp);
}

SEXP R_FUN_NAME(rtools_test, field_mat_double)(SEXP exp) {
  return rtools_test_field<arma::Mat<double> >(exp);
}

SEXP R_FUN_NAME(rtools_test, field_col_double)(SEXP exp) {
  return rtools_test_field<arma::Col<double> >(exp);
}

SEXP R_FUN_NAME(rtools_test, field_col_u32)(SEXP exp) {
  return rtools_test_field<arma::Col<arma::u32> >(exp);
}

SEXP R_FUN_NAME(rtools_test, field_col_s32)(SEXP exp) {
  return rtools_test_field<arma::Col<arma::s32> >(exp);
}

SEXP R_FUN_NAME(rtools_test, field_sp_mat)(SEXP exp) {
  return rtools_test_field<arma::sp_mat>(exp);
}

//NOTE rlist
