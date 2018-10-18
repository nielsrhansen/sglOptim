// Reset macros

#ifdef MODULE_NAME
  #undef MODULE_NAME
  #undef OBJECTIVE
  #undef PREDICTOR
#endif

// Module name
#define MODULE_NAME MODULE

#define OBJECTIVE MODULE

#include <sgl/RInterface/sgl_lambda_seq.h>
#include <sgl/RInterface/sgl_fit.h>
#include <sgl/RInterface/sgl_test.h>

#define PREDICTOR sgl::LinearPredictor < MATRIX , sgl::LinearResponse >

#include <sgl/RInterface/sgl_predict.h>
#include <sgl/RInterface/sgl_subsampling.h>
