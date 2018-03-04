#ifndef TLO_UTIL_HPP
#define TLO_UTIL_HPP

#include <cstddef>
#include <sstream>
#include <string>

namespace tlo {
namespace detail {
extern unsigned long allocateCount;
extern unsigned long deallocateCount;
extern std::size_t totalByteCount;
}  // namespace detail

template <typename ValueType>
class CountingAllocator {
 public:
  using value_type = ValueType;

  CountingAllocator() noexcept {}  // not required, unless used
  template <typename OtherType>
  CountingAllocator(const CountingAllocator<OtherType> &) noexcept {}

  ValueType *allocate(std::size_t count) {
    ++detail::allocateCount;
    std::size_t byteCount = count * sizeof(ValueType);
    void *bytes = ::operator new(byteCount);
    detail::totalByteCount += byteCount;
    return static_cast<ValueType *>(bytes);
  }

  void deallocate(ValueType *bytes, std::size_t) noexcept {
    ++detail::deallocateCount;
    ::operator delete(bytes);
  }
};

template <typename ValueType, typename OtherType>
bool operator==(const CountingAllocator<ValueType> &,
                const CountingAllocator<OtherType> &) noexcept {
  return true;
}

template <typename ValueType, typename OtherType>
bool operator!=(const CountingAllocator<ValueType> &x,
                const CountingAllocator<OtherType> &y) noexcept {
  return !(x == y);
}

void countingAllocatorResetCounts();
unsigned long countingAllocatorAllocateCount();
unsigned long countingAllocatorDeallocateCount();
std::size_t countingAllocatorTotalByteCount();
void countingAllocatorPrintCounts();

class IntPtr {
 public:
  int *ptr;
  IntPtr();
  IntPtr(const IntPtr &other);
  IntPtr(IntPtr &&other);
  ~IntPtr();
};

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
