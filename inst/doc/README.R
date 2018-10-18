params <-
structure(list(pkg = "sglOptim"), .Names = "pkg")

## ---- echo = FALSE-------------------------------------------------------
set.seed(150)

pkg_version <- packageVersion(params$pkg)

x_dcf <- read.dcf(file = file.path(getwd(), "..", "DESCRIPTION"))

if("GitHubRepo" %in% colnames(x_dcf)) {
  pkg_branch <- x_dcf[1,"GitHubRepo"]

  pkg_version_type <- switch(pkg_branch,
    release = "release",
    master = "release candidate",
    develop = "development version"
  )
} else {
  pkg_branch <- "release"
  pkg_version_type <- "release"
}

## ----eval = FALSE--------------------------------------------------------
#  install.packages("sglOptim")

## ----eval = FALSE--------------------------------------------------------
#  # install.packages("devtools")
#  devtools::install_github("vincent-dk/sglOptim")

## ----eval = FALSE--------------------------------------------------------
#  # install.packages("devtools")
#  devtools::install_github("vincent-dk/sglOptim", ref = "develop")

