#ifndef ATM_VECTOR_H
#define ATM_VECTOR_H

#include "r-common.h"
#include "sxp-utils.h"
#include "sxp-protection.h"

namespace rcc{


template <typename T, typename S>
class atm_vector;

template <typename T, typename S = preserved_t>
class atm_vector_base
{
 protected:
  size_t n = 0;
  S s = sxp_null;
  T* pointer = nullptr;

 public:
  using value_type = T;
  using iterator = T*;
  using const_iterator = const T*;

  atm_vector_base()
  { }
  atm_vector_base(sexp* x) :
      n{ sxp_length(x) },
      s{ x },
      pointer{ sxp_pointer<T>(x) }
  {
    int type = vec_traits<T>::sxp_type;
    if (sxp_type(x) != type) {
      const char* type_name = sxp_type_name(type).c_str();
      r_throw("Expected type %s\n", type_name);
    }
  }
  atm_vector_base(int size) :
      n{ (size_t) size },
      s{ vec_alloc<T>(n) },
      pointer{ sxp_pointer<T>(s) }
  { }
  atm_vector_base(atm_vector<T, S>& other) :
      n{ other.size() },
      s{ vec_alloc<T>(n) },
      pointer{ sxp_pointer<T>(s) }
  { std::copy(other.begin(), other.end(), pointer); }
  atm_vector_base(atm_vector<T, S>&& other) :
      n{ other.size() },
      s{ std::move((sexp*) other) },
      pointer{ other.pointer }
  { }
  atm_vector_base(iterator begin, iterator end) :
      n{ (size_t) std::distance(begin, end) },
      s{ vec_alloc<T>(n) },
      pointer{ sxp_pointer<T>((sexp*) s) }
  { std::copy(begin, end, pointer); }
  atm_vector_base(std::initializer_list<T> init) :
      n{ (size_t) init.size() },
      s{ vec_alloc<T>(n) },
      pointer{ sxp_pointer<T>((sexp*) s) }
  { std::copy(init.begin(), init.end(), pointer); }

  operator sexp* const () const { return (sexp*) s; }

  iterator begin() { return pointer; }
  iterator end() { return pointer + n; }

  const_iterator begin() const { return pointer; }
  const_iterator end() const { return pointer + n; }

  size_t size() const { return n; }
  size_t length() const { return n; }

  T& operator[](size_t i) { return *(pointer + i); }
  const T& operator[](size_t i) const { return *(pointer + i); }
};


template <typename T, typename S = preserved_t>
class atm_vector : public atm_vector_base<T, S>
{ using atm_vector_base<T, S>::atm_vector_base; };

template <typename S>
class atm_vector<const char, S> : public atm_vector_base<const char, sexp*>
{
  using atm_vector_base<const char, sexp*>::atm_vector_base;

 public:
  atm_vector(std::string x)
  {
    n = x.size();
    s = Rf_mkChar(x.c_str());
    pointer = sxp_pointer<const char>(s);
  }
  atm_vector(sexp* x)
  {
    int type = sxp_type(x);
    if (type == STRSXP) {
      if (sxp_length(x) != 1) {
        r_throw("Only scalar character vectors can be treated as strings");
      }
      x = chr_get(x, 0);
    } else if (type != CHARSXP && type != 73) {
      const char* type_name = sxp_type_name(type).c_str();
      r_throw("Expected string-like type, not %s\n", type_name);
    }

    n = sxp_length(x);
    s = x;
    pointer = sxp_pointer<const char>(x);
  }

  const char* c_str() { return pointer; }
};


using r_dbl = atm_vector<double>;
using r_int = atm_vector<int>;
using r_cpl = atm_vector<Rcomplex>;

using r_free_dbl = atm_vector<double, sexp*>;
using r_free_int = atm_vector<int, sexp*>;
using r_free_cpl = atm_vector<Rcomplex, sexp*>;

using sxp_string = atm_vector<const char, sexp*>;


template <typename T>
inline
T* sxp_pointer(atm_vector<T> x) {
  return x.begin();
}

double na_value(r_dbl x) { return R_NaReal; };
int na_value(r_int x) { return R_NaInt; };


} // namespace rcc

#endif
