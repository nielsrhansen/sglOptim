# on windows in terminal do : SET NOT_CRAN=true

get_script_path <- function() {
  cmd.args <- commandArgs()
  m <- regexpr("(?<=^--file=).+", cmd.args, perl=TRUE)
  
  script.dir <- dirname(regmatches(cmd.args, m))
  
  if(length(script.dir) == 0) {
    stop("can't determine script dir: please call the script with Rscript")
  }
  
  if(length(script.dir) > 1) {
    stop("can't determine script dir: more than one '--file' argument detected")
  }
  
  return(script.dir)
}

package_name <- function(path) {
  out <- c(read.dcf(list.files(path, pattern="DESCRIPTION",
                               recursive=TRUE, full.names=TRUE), "Package"))
  return(out)
}

build_check_local <- function(pkg, path) {
  ver <- packageVersion(pkg, lib.loc = path)
  
  system2("R",
          args = c("CMD", "build", file.path(path, pkg))
  )
  
  build_name <- paste(pkg, "_", ver, ".tar.gz", sep="")
  
  system2("R",
          args = c("CMD", "check --as-cran", build_name)
  )
}

if( ! "roxygen2" %in% rownames(installed.packages())) {
  install.packages("roxygen2", repos = "https://cloud.r-project.org")
}

library("roxygen2")

path <- file.path(getwd(), get_script_path())
pkg <- package_name(path)

roxygenise(path)
print(warnings())

build_check_local(pkg, file.path(path, ".."))