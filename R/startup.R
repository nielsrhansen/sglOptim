.onAttach <- function(libname, pkgname) {

	c.config <- sgl.c.config()

	if(c.config$debugging) packageStartupMessage("sglOptim: Compiled with debugging on -- this may slow down runtime")

}
