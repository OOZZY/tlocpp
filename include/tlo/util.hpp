#ifndef TLO_UTIL_HPP
#define TLO_UTIL_HPP

#include <sstream>

namespace tlo {
// - reverse at()
// - if index == 0, returns last elem
// - if index == 1, returns 2nd last elem
// - if index == 2, returns 3rd last elem
// - etc.
template <typename Container>
auto &rat(Container &container, std::size_t index) {
  return container[container.size() - 1 - index];
}

template <typename Container>
const auto &rat(const Container &container, std::size_t index) {
  return container[container.size() - 1 - index];
}

template <typename Value>
std::string toString(const Value &value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}
}  // namespace tlo

#endif  // TLO_UTIL_HPP
