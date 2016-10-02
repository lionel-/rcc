#ifndef SXP_VECTOR_H
#define SXP_VECTOR_H

#include <algorithm>
#include <vector>
#include <iterator>
#include <string>
#include <initializer_list>

#include "r-common.h"
#include "rcc-utils.h"
#include "sxp-utils.h"
#include "sxp-protection.h"

namespace rcc {


// FIXME: should probably just pass sexp* or const char* instead of
// methods structs, then specialise a set of functions on those types.
struct generic_methods_t {
  sexp* alloc(size_t n) { return vec_alloc<sexp*>(n); }
  sexp* get_elt(sexp* x, int i) const { return lst_get(x, i); };
  void set_elt(sexp* x, int i, sexp* y) { lst_set(x, i, y); }
};
struct string_methods_t {
  sexp* alloc(size_t n) { return vec_alloc<const char*>(n); }
  sexp* get_elt(sexp* x, int i) const { return chr_get(x, i); };
  void set_elt(sexp* x, int i, sexp* y) { chr_set(x, i, y); }
};

template <typename T,
          typename M,
          typename S = preserved_t>
class sxp_vector_base : public M
{
 protected:
  const size_t n = 0;
  S s = sxp_null;
  std::vector<T> elements;

  using M::get_elt;
  using M::set_elt;
  using M::alloc;

 public:
  using value_type = T;
  using iterator = typename std::vector<T>::iterator;
  using const_iterator = typename std::vector<T>::const_iterator;

  sxp_vector_base(sexp* x) :
      n{ sxp_length(x) },
      s{ x }
  {
    if (sxp_type(x) != VECSXP && sxp_type(x) != STRSXP)
      r_throw("Expected vector of sexp objects\n");

    elements.reserve(n);
    for (int i = 0; i < n; ++i)
      elements.push_back(get_elt(s, i));
  }

  sxp_vector_base(std::initializer_list<sexp*> init) :
      n{ (size_t) init.size() },
      s{ alloc(n) }
  {
    // static_caster() wraps the sexp's with a known data type
    elements.reserve(n);
    std::transform(init.begin(), init.end(),
                   std::back_inserter(elements),
                   utils::static_caster<sexp*, T>());
  }
  sxp_vector_base(std::initializer_list<std::string> init) :
      n{ (size_t) init.size() },
      s{ alloc(n) }
  {
    elements.reserve(n);
    std::transform(init.begin(), init.end(),
                   std::back_inserter(elements),
                   utils::static_caster<std::string, T>());
  }

  // Keep actual sexp in sync with represented elements
  void sync_sexp() {
    auto it = begin();
    for (int i = 0; i < n; ++i, ++it)
      set_elt(s, i, (sexp*) *it);
  }
  operator sexp* () {
    sync_sexp();
    return s;
  }
  operator sexp* const () const {
    return s;
  }

  iterator begin() { return elements.begin(); }
  iterator end() { return elements.end(); }

  const_iterator begin() const { return elements.begin(); }
  const_iterator end() const { return elements.end(); }
};

template <typename T, typename S = preserved_t>
class sxp_vector : public sxp_vector_base<T, generic_methods_t, S>
{ using sxp_vector_base<T, generic_methods_t, S>::sxp_vector_base; };

template <typename S>
class sxp_vector<sxp_string, S> : public sxp_vector_base<sxp_string, string_methods_t, S>
{ using sxp_vector_base<sxp_string, string_methods_t, S>::sxp_vector_base; };


class rcc_t;
using r_lst = sxp_vector<rcc_t, preserved_t>;
using r_chr = sxp_vector<sxp_string, preserved_t>;

using r_lst_int = sxp_vector<r_free_int, preserved_t>;
using r_lst_dbl = sxp_vector<r_free_dbl, preserved_t>;


} // namespace rcc

#endif
