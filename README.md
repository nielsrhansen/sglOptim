## Sparse Group Lasso Solver

This is the R package sglOptim version **1.3.7**.

### R-package Overview

This package doesn’t provide any direct functionality. It is a service
package for other R packages, and it provides an implementation of an
optimizer of sparse group lasso penalized objective functions. The
package is used by the following other R packages:

  - For fitting **multivariate linear regression** models using sparse
    group lasso see the [lsgl](https://github.com/nielsrhansen/lsgl) R
    package.
  - For fitting **multinomial classification models** using sparse group
    lasso see the [msgl](https://github.com/nielsrhansen/msgl) R
    package.

More details on the algorithm are given in the paper [Sparse group lasso
and high dimensional multinomial
classification](http://dx.doi.org/10.1016/j.csda.2013.06.004).

The loss/objective function must be defined in a C++ module.

### Installation

Install the released version from CRAN:

``` r
install.packages("sglOptim")
```

Install the version from GitHub:

``` r
# install.packages("devtools")
devtools::install_github("nielsrhansen/sglOptim", build_vignettes = TRUE)
```

If you don’t want to build the vignettes when installing, just remove
the `build_vignettes = TRUE` argument.

### Author

Martin Vincent wrote the package. Niels Richard Hansen is the current
maintainer.

### License

GPL (\>=2)
