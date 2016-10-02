#ifndef SXP_UTILS_H
#define SXP_UTILS_H

#include "r-common.h"

namespace rcc {


// Defined in r-internals.cpp
void* sxp_pointer(sexp* x);
sexp* lst_get(sexp* x, size_t i);
sexp* lst_set(sexp* x, size_t i, sexp* y);
sexp* chr_get(sexp* x, size_t i);
void chr_set(sexp* x, size_t i, sexp* y);

template <typename T> struct vec_traits { };
template <> struct vec_traits<int> { enum{ sxp_type = INTSXP }; };
template <> struct vec_traits<double> { enum{ sxp_type = REALSXP }; };
template <> struct vec_traits<Rcomplex> { enum{ sxp_type = CPLXSXP }; };
template <> struct vec_traits<sexp*> { enum{ sxp_type = VECSXP }; };
template <> struct vec_traits<const char> { enum{ sxp_type = 73 }; }; // FIXME: fragile
template <> struct vec_traits<const char*> { enum{ sxp_type = STRSXP }; };

template <typename T>
inline
sexp* vec_alloc(size_t n) {
  return Rf_allocVector(vec_traits<T>::sxp_type, n);
}

inline
size_t sxp_length(sexp* x) {
  return Rf_length(x);
}

inline
sexp_e sxp_type(sexp* x) {
  return TYPEOF(x);
}

template <typename T>
inline
T* sxp_pointer(sexp* x) {
  return (T*) sxp_pointer(x);
}

// FIXME: what about non-array types?
template <typename T>
inline
sexp* sxp_make_scalar(T x) {
  sexp* s = vec_alloc<T>(1);
  T* array = (T*) sxp_pointer(s);
  array[0] = x;
  return s;
}

inline
sexp* sxp_copy(sexp* x) {
  return Rf_duplicate(x);
}

inline
std::string sxp_type_name(int x) {
  switch (x) {
  case NILSXP:     return "NILSXP";
  case SYMSXP:     return "SYMSXP";
  case LISTSXP:    return "LISTSXP";
  case CLOSXP:     return "CLOSXP";
  case ENVSXP:     return "ENVSXP";
  case PROMSXP:    return "PROMSXP";
  case LANGSXP:    return "LANGSXP";
  case SPECIALSXP: return "SPECIALSXP";
  case BUILTINSXP: return "BUILTINSXP";
  case CHARSXP:    return "CHARSXP";
  case LGLSXP:     return "LGLSXP";
  case INTSXP:     return "INTSXP";
  case REALSXP:    return "REALSXP";
  case CPLXSXP:    return "CPLXSXP";
  case STRSXP:     return "STRSXP";
  case DOTSXP:     return "DOTSXP";
  case ANYSXP:     return "ANYSXP";
  case VECSXP:     return "VECSXP";
  case EXPRSXP:    return "EXPRSXP";
  case BCODESXP:   return "BCODESXP";
  case EXTPTRSXP:  return "EXTPTRSXP";
  case WEAKREFSXP: return "WEAKREFSXP";
  case S4SXP:      return "S4SXP";
  case RAWSXP:     return "RAWSXP";
  default:         return "<unknown>";
  }
}

inline
std::string sxp_type_name(sexp* x) {
  return sxp_type_name(sxp_type(x));
}


} // namespace rcc

#endif
