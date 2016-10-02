#ifndef RCC_GENERIC_H
#define RCC_GENERIC_H

#include <memory>

#include "r-common.h"
#include "rcc-utils.h"
#include "sxp-utils.h"
#include "atm-vector.h"
#include "sxp-vector.h"

namespace rcc {


// FIXME: Should this be sxp_names() and sxp_length()?

template <typename T>
sexp* names(const T& x) {
  return Rf_getAttrib((sexp*) x, R_NamesSymbol);
}

template <typename T>
size_t length(const T& x) {
  return sxp_length((sexp*) x);
}

template <typename T>
sexp* subset(T x, const int index) {
  return sxp_null;
}

class rcc_t
{
 public:
  rcc_t() : self_(std::make_shared<rcc_model_t<sexp*>>(sxp_null))
  { }
  template <typename T>
  rcc_t(T x) : self_(std::make_shared<rcc_model_t<T>>(x))
  { }

  // Dispatch methods
  friend sexp* names(const rcc_t& x) { return x.self_->names_(); }
  sexp* operator[] (const int index) { return this->self_->subset_(index); }
  operator sexp* () const { return this->self_->get_(); }

 private:
  struct rcc_interface_t
  {
    virtual ~rcc_interface_t() = default;
    virtual sexp* get_() const = 0;
    virtual sexp* subset_(const int index) const = 0;
    virtual sexp* names_() const = 0;
  };

  template <typename T>
  struct rcc_model_t : rcc_interface_t
  {
    // Methods should be called unqualified to ensure ADL scoping
    rcc_model_t(T x) : rcc_object(x) { }
    sexp* get_() const { return (sexp*) rcc_object; };
    sexp* subset_(const int index) const { return subset(rcc_object, index); }
    sexp* names_() const { return names(rcc_object); }
    T rcc_object;
  };

  std::shared_ptr<const rcc_interface_t> self_;
};

class rcc_t;

template <>
rcc_t::rcc_t(sexp* x)
{
  switch(sxp_type(x)) {
  case INTSXP: self_ = std::make_shared<rcc_model_t<r_int>>(x); break;
  case REALSXP: self_ = std::make_shared<rcc_model_t<r_dbl>>(x); break;
  case CPLXSXP: self_ = std::make_shared<rcc_model_t<r_cpl>>(x); break;
  case VECSXP: self_ = std::make_shared<rcc_model_t<r_lst>>(x); break;
  case STRSXP: self_ = std::make_shared<rcc_model_t<r_chr>>(x); break;
  default: r_throw("Unimplemented rcc_t wrapper");
  }
}


} // namespace rcc

#endif
