\name{NEWS}
\title{News for Package 'sglOptim'}


\section{Changes in sglOptim version 1.3.7 (2018-10-18)}{
\itemize{
\item Original author and previous maintainer, Martin Vincent, has stopped development. This version is released by a new maintainer. Changes from version 1.3.6 are only to comply to CRAN policies.
}
}

\section{Changes in sglOptim version 1.3.6 (2017-03-03)}{
\itemize{
\item Bug fixes
}
}
       
\section{Changes in sglOptim version 1.3.5 (2016-12-28)}{
\itemize{
\item Internal code cleanup, bug fixes and refactoring.
}
}

\section{Changes in sglOptim version 1.3.0 (2016-09-28)}{
\itemize{
\item Improved documentation and functionality:
\itemize{
	\item added functions for extracting number of nonzero features and parameters (\code{feature_stat} and \code{parameter_stat})
	\item Minor updates and documentation corrections
	\item Better handling of R memory protection (fewer PROTECT calls)
	\item Hadling of more complex response types
	\item subsampling uses foreach and doParallel (showing progressbar no longer supported for subsampling and cross validation)
	\item subsampling now supports different lambda sequences for each subsample
}
 \item Fixes :
     \itemize{
     	\item fixed sample check bug
	\item minor fixes for some numerical computations
     }

\item API changes :
		\itemize{
		 \item all lineaer loss classes must set (a sgl::natural) n_variables, whihch will determine the size of the lineaer preditors ( n_samples x n_variables)
		 \item in class MultiResponse n_groups -> n_responses
		}
}
}

\section{Changes in sglOptim version 1.2.0 (2015-09-16)}{
  \itemize{
  \item Improved flexibility and reliability :
  \itemize{
    \item added handling of q = 1 cases (each feature has one parameter)
    \item correct handling of non penalized parameters when computing lambda.max (fit non penalized parameters before computing critical lambdas)
    \item increased speed when optimizing with a high number of groups/classes (> 100)
    \item some improvements on error reporting and argument validation
   }

   \item Fixes :
     \itemize{
        \item fixed bug related to grouping
        \item fixed seval minor bugs
        \item fixed startup messages
     }
  }
}
