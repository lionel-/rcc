#ifndef SXP_PROTECTION_H
#define SXP_PROTECTION_H

#include "r-common.h"

namespace rcc {


void sxp_protect(sexp* x) {
  PROTECT(x);
}
void sxp_unprotect(sexp* x) {
  UNPROTECT(1);
}

void sxp_preserve(sexp* x) {
  R_PreserveObject(x);
}
void sxp_release(sexp* x) {
  R_ReleaseObject(x);
}

template <void (*Shield)(sexp* x), void (*Unshield)(sexp* x)>
struct shield_t
{
  sexp* s;

  shield_t(sexp* x) : s{ x } { Shield(s); }
  ~shield_t() { Unshield(s); }

  template <typename T>
  shield_t(const T& x) : s{ (sexp*) x } { Shield(s); }

  template <typename T>
  shield_t& operator= (const T& x) {
    s = (sexp*) x;
    Shield(s);
    return *this;
  }

  template <typename T>
  shield_t& operator= (const T&& x) {
    s = (sexp*) x;
    Shield(s);
    return *this;
  }

  operator sexp* () const { return s; }
};

using preserved_t = shield_t<&sxp_preserve, &sxp_release>;
using protected_t = shield_t<&sxp_protect, &sxp_unprotect>;


}

#endif
