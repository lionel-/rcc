#ifndef R_COMMON_H
#define R_COMMON_H

#define R_NO_REMAP
#include <Rinternals.h>

// Avoids warning in case Rcpp is included
#undef R_NO_REMAP

namespace rcc {


using sexp = SEXPREC;
using sexp_e = SEXPTYPE;
using size_t = R_len_t;

#define sxp_null R_NilValue

template <typename... Args>
void r_throw(const char* msg, Args&& ...args) {
  Rf_error(msg, args...);
}

template <typename... Args>
inline
void r_printf(const char* msg, Args&& ...args) {
  Rprintf(msg, args...);
}


}

#endif
