#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstddef>

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
}  // namespace tlo

#endif  // UTIL_HPP
