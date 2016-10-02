#ifndef R_DEBUG_H
#define R_DEBUG_H

#include <typeinfo>
#include <string>

#include "r-common.h"
#include "sxp-utils.h"

namespace rcc {


inline
void dbg_print(sexp* x) {
  Rf_PrintValue(x);
}

template <typename T>
inline
void dbg_ctype(T x) {
  const char* type = typeid(T).name();
  r_printf("C type: %s\n", type);
}
template <typename T>
inline
void dbg_ctype() {
  const char* type = typeid(T).name();
  r_printf("C type: %s\n", type);
}

inline
void dbg_rtype(sexp* x) {
  std::string type_name = sxp_type_name(x);
  r_printf("R type: %s\n", type_name.c_str());
}

inline
void dbg_str(sexp* x) {
  sexp* str_call = Rf_lang2(Rf_install("str"), x);
  dbg_print(Rf_eval(str_call, R_GlobalEnv));
}

inline
void dbg_inspect(sexp* x) {
  sexp* inspect_call;
  inspect_call = Rf_lang2(Rf_install("inspect"), x);
  inspect_call = Rf_lang2(Rf_install(".Internal"), inspect_call);
  dbg_print(Rf_eval(inspect_call, R_GlobalEnv));
}


}

#endif
