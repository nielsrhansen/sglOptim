## Run from package root directory. The script builds the 
## vignettes and includes the md-file for publication on GitHub.

vignettes.path <- file.path(getwd(), "vignettes")
vignettes.files <- list.files(vignettes.path, pattern="*.Rmd")

for(file in vignettes.files) {
  
  input_file <- file.path(vignettes.path, file)
  
  rmarkdown::render(
    input = input_file,
    output_format = rmarkdown::md_document(variant = "gfm")
  )
  
}


# Move generated files to pkg root
files_to_move <- list.files(vignettes.path)
files_to_move <- files_to_move[ ! files_to_move %in% vignettes.files ]

for(file in files_to_move) {
  
  from <- file.path(vignettes.path, file)
  to <- file.path(getwd(), file)
  
  unlink(to, recursive = TRUE)
  file.rename(from, to)
}
