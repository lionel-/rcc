
/*

  This file contains function definitions that rely on functions and
  macros provided only when USE_RINTERNALS is defined. This can cause
  problems when including files from other projects like Rcpp so we
  provide definitions relying on USE_RINTERNALS in this separate file.

  It should typically not be included in your regular cpp files.
  Instead, it should be included in a separate cpp file to be treated
  in a separate compilation unit.

  FIXME: Better solution?

 */

#define R_NO_REMAP
#define USE_RINTERNALS
#include <Rinternals.h>

#include "r-common.h"

namespace rcc {


void* sxp_pointer(sexp* x) {
  return DATAPTR(x);
}

sexp* lst_get(sexp* x, size_t i) {
  return VECTOR_ELT(x, i);
}
sexp* lst_set(sexp* x, size_t i, sexp* y) {
  return SET_VECTOR_ELT(x, i, y);
}

sexp* chr_get(sexp* x, size_t i) {
  return STRING_ELT(x, i);
}
void chr_set(sexp* x, size_t i, sexp* y) {
  SET_STRING_ELT(x, i, y);
}


}
