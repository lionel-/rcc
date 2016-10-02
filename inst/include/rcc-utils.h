#ifndef RCC_UTILS_H
#define RCC_UTILS_H

namespace rcc { namespace utils {


template <typename T1, typename T2>
struct static_caster
{
  T2 operator() (T1 x) {
    return static_cast<T2>(x);
  }
};

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&& ...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


}} // namespace rcc::internal

#endif
