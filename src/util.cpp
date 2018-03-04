#include "tlo/util.hpp"
#include <iostream>

namespace tlo {
namespace detail {
unsigned long allocateCount = 0;
unsigned long deallocateCount = 0;
std::size_t totalByteCount = 0;
}  // namespace detail

void countingAllocatorResetCounts() {
  detail::allocateCount = 0;
  detail::deallocateCount = 0;
  detail::totalByteCount = 0;
}

unsigned long countingAllocatorAllocateCount() { return detail::allocateCount; }

unsigned long countingAllocatorDeallocateCount() {
  return detail::deallocateCount;
}

std::size_t countingAllocatorTotalByteCount() { return detail::totalByteCount; }

void countingAllocatorPrintCounts() {
  std::cout << "allocate count: " << detail::allocateCount << std::endl;
  std::cout << "deallocate count: " << detail::deallocateCount << std::endl;
  std::cout << "Total bytes allocated: " << detail::totalByteCount << std::endl;
}

IntPtr::IntPtr() : ptr(new int) {}
IntPtr::IntPtr(const IntPtr &other) : ptr(new int(*other.ptr)) {}
IntPtr::IntPtr(IntPtr &&other) : ptr(other.ptr) { other.ptr = nullptr; }
IntPtr::~IntPtr() {
  if (!ptr) {
    return;
  }
  delete ptr;
  ptr = nullptr;
}
}  // namespace tlo
