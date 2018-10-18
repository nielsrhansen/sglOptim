/*
	Sgl template library for optimizing sparse group lasso penalized objectives.
    Copyright (C) 2012 Martin Vincent

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/

#ifndef ERROR_H_
#define ERROR_H_

//Error msg
std::string numerical_error_msg("The algorithm has encountered a numerical problem\n   Try/check the following:\n   (1) Check the lambda sequence and try with a longer sequence (the length d should be larger than 100) \n   (2) try with a larger lambda.min\n   (3) Ensure minimum one sample from each group/class present in all subsample (or cross validation) sets used \n");
std::string convege_error_msg("The algorithm has encountered a convergence problem\n   Try/check the following:\n   (1) Check the lambda sequence and try with a longer sequence (the length d should be larger than 100) \n   (2) try with a larger lambda.min\n   (3) Ensure minimum one sample from each group/class present in all subsample (or cross validation) sets used \n");


#ifdef SGL_CONVERGENCE_CHECK
#define CONVERGENCE_CHECK(limit) sgl::natural convergence_itr = 0; sgl::natural convergence_limit = limit;
#define CONVERGENCE_CHECK_INCREASE ++convergence_itr; if(convergence_itr > convergence_limit) throw std::runtime_error(create_error_msg(sgl::convege_error_msg.c_str(), __FILE__, __LINE__));
#else
#define CONVERGENCE_CHECK(limit)
#define CONVERGENCE_CHECK_INCREASE
#endif

#ifdef SGL_CATCH_EXCEPTIONS
#define SGL_EXCEPTIONS std::exception
#define SGL_EXCEPTIONS_GENRAL ...
#else
#define SGL_EXCEPTIONS dummy_exception_a
#define SGL_EXCEPTIONS_GENRAL dummy_exception_b
#endif

#ifndef SGL_ERROR
#define SGL_ERROR(msg) report_error(msg);
#endif

#ifndef SGL_STD_OUT
#define SGL_STD_OUT Rcpp::Rcout
#endif

static bool sgl_interrupt = false;

#ifndef SGL_INTERRUPT_CHECK
#define SGL_INTERRUPT_CHECK if(sgl_interrupt) throw std::runtime_error("");
#endif

#ifndef SGL_INTERRUPT_RESET
#define SGL_INTERRUPT_RESET sgl_interrupt = false
#endif

#ifndef SGL_INTERRUPT
#define SGL_INTERRUPT sgl_interrupt = true;
#endif

#ifndef SGL_TRY
#define SGL_TRY try
#endif

#ifndef SGL_CATCH_ERROR
#define SGL_CATCH_ERROR catch (std::exception & e) { \
	if(e.what() != NULL) { 	\
		SGL_ERROR(e.what());	\
	}	\
\
	else {	\
		SGL_ERROR("Unknown error"); \
	} \
\
} catch (...) { \
	SGL_ERROR("Unknown error"); \
} \
\
return R_NilValue; //Avoid compiler warnings
#endif



//Debug

std::string create_error_msg(
		const char * msg,
		const char * file_name,
		int line_number) {

	std::ostringstream error_msg;

  error_msg << msg <<	" (Assert failed in " <<
		file_name << " at line " <<	line_number << " )";

  return error_msg.str();
}

#ifdef SGL_RUNTIME_CHECKS
	#define ASSERT_IS_FINITE(x) if(!sgl::is_finite(x)) throw std::runtime_error(sgl::create_error_msg(sgl::numerical_error_msg.c_str(), __FILE__, __LINE__));
	#define ASSERT_IS_NUMBER(x) if(x != x) throw std::runtime_error(sgl::create_error_msg(sgl::numerical_error_msg.c_str(), __FILE__, __LINE__));
	#define ASSERT_IS_POSITIVE(x) if(x <= 0) throw std::runtime_error(sgl::create_error_msg(sgl::numerical_error_msg.c_str(), __FILE__, __LINE__));
	#define ASSERT_IS_NON_NEGATIVE(x) if(x < 0) throw std::runtime_error(sgl::create_error_msg(sgl::numerical_error_msg.c_str(), __FILE__, __LINE__));
#else
	#define ASSERT_IS_FINITE(x) //do nothing
	#define ASSERT_IS_NUMBER(x) //do nothing
	#define ASSERT_IS_POSITIVE(x) //do nothing
	#define ASSERT_IS_NON_NEGATIVE(x) //do nothing
#endif

#ifdef SGL_DEBUG_SIMPLE
	#define ASSERT_IS_ZERO(x) if(accu(x != 0) > 0) throw std::runtime_error(sgl::create_error_msg(sgl::numerical_error_msg.c_str(), __FILE__, __LINE__));
	#define ASSERT_IS_NON_ZERO(x) if(accu(x != 0) == 0) throw std::runtime_error(sgl::create_error_msg(sgl::numerical_error_msg.c_str(), __FILE__, __LINE__));
#else
	#define ASSERT_IS_ZERO(x) //do nothing
	#define ASSERT_IS_NON_ZERO(x) //do nothing
#endif

#ifdef SGL_DEBUG_COMPLEX
	#define SGL_DEBUG_BLOCK_ACTIVE
	#define SGL_DEBUG_GB
#endif

#ifdef SGL_DEBUG_INFO_ALL
	#define SGL_DEBUG_INFO_GB_OPT
	#define SGL_DEBUG_INFO_STEPSIZE
	#define SGL_DEBUG_INFO_QUADRATIC
#endif

#endif /* ERROR_H_ */
