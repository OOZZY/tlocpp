#ifndef TLO_DARRAY_HPP
#define TLO_DARRAY_HPP

#include <cassert>
#include <cstring>
#include <memory>

namespace tlo {
/*
 * - dynamic array
 */
template <typename ValueType_,
          typename AllocatorType_ = std::allocator<ValueType_>>
class DArray {
 public:
  using ValueType = ValueType_;
  using AllocatorType = AllocatorType_;

 private:
  AllocatorType allocator;
  ValueType *bytes;
  std::size_t size_;
  std::size_t capacity_;

 public:
  bool isValid() const { return size_ <= capacity_; }

  DArray(std::size_t capacity = 0)
      : bytes(capacity ? std::allocator_traits<AllocatorType>::allocate(
                             allocator, capacity)
                       : nullptr),
        size_(0),
        capacity_(capacity) {}

 private:
  void pushBackAllElementsOfOther(const DArray &other) {
    for (std::size_t i = 0; i < other.size_; ++i) {
      pushBack(other.bytes[i]);
    }
  }

 public:
  DArray(const DArray &other) : DArray(other.capacity_) {
    assert(other.isValid());
    pushBackAllElementsOfOther(other);
  }

 private:
  void destructAllElements() {
    for (std::size_t i = 0; i < size_; ++i) {
      ValueType *element = bytes + i;
      std::allocator_traits<AllocatorType>::destroy(allocator, element);
    }
  }

 public:
  ~DArray() {
    assert(isValid());

    if (!bytes) {
      return;
    }

    destructAllElements();

    std::allocator_traits<AllocatorType>::deallocate(allocator, bytes,
                                                     capacity_);
    bytes = nullptr;
  }

  DArray &operator=(const DArray &other) {
    assert(isValid());
    assert(other.isValid());

    DArray copy(other);

    unsigned char temp[sizeof(DArray)];
    std::memcpy(temp, this, sizeof(DArray));
    std::memcpy(this, &copy, sizeof(DArray));
    std::memcpy(&copy, temp, sizeof(DArray));

    return *this;
  }

  std::size_t size() const {
    assert(isValid());

    return size_;
  }

  std::size_t capacity() const {
    assert(isValid());

    return capacity_;
  }

  bool isEmpty() const {
    assert(isValid());

    return size_ == 0;
  }

  const ValueType &operator[](std::size_t index) const {
    assert(isValid());
    assert(!isEmpty());
    assert(index < size_);

    return bytes[index];
  }

  ValueType &operator[](std::size_t index) {
    assert(isValid());
    assert(!isEmpty());
    assert(index < size_);

    return bytes[index];
  }

  const ValueType &front() const {
    assert(isValid());
    assert(!isEmpty());

    return bytes[0];
  }

  ValueType &front() {
    assert(isValid());
    assert(!isEmpty());

    return bytes[0];
  }

  const ValueType &back() const {
    assert(isValid());
    assert(!isEmpty());

    return bytes[size_ - 1];
  }

  ValueType &back() {
    assert(isValid());
    assert(!isEmpty());

    return bytes[size_ - 1];
  }

 private:
  static const std::size_t STARTING_CAPACITY = 1;

  void allocateBytesIfNeeded() {
    if (!bytes) {
      bytes = std::allocator_traits<AllocatorType>::allocate(allocator,
                                                             STARTING_CAPACITY);
      capacity_ = STARTING_CAPACITY;
    }
  }

  void resizeBytesIfNeeded() {
    if (size_ == capacity_) {
      std::size_t newCapacity = capacity_ * 2;
      ValueType *newBytes = std::allocator_traits<AllocatorType>::allocate(
          allocator, newCapacity);

      std::memcpy(newBytes, bytes, size_ * sizeof(ValueType));

      std::allocator_traits<AllocatorType>::deallocate(allocator, bytes,
                                                       capacity_);
      bytes = newBytes;
      capacity_ = newCapacity;
    }
  }

  void pushBackData(const ValueType &data) {
    ValueType *destination = bytes + size_;
    std::allocator_traits<AllocatorType>::construct(allocator, destination,
                                                    data);
    ++size_;
  }

 public:
  void pushBack(const ValueType &data) {
    assert(isValid());

    allocateBytesIfNeeded();
    resizeBytesIfNeeded();
    pushBackData(data);
  }

 private:
  void pushBackData(ValueType &&data) {
    ValueType *destination = bytes + size_;
    std::allocator_traits<AllocatorType>::construct(allocator, destination,
                                                    std::move(data));
    ++size_;
  }

 public:
  void pushBack(ValueType &&data) {
    assert(isValid());

    allocateBytesIfNeeded();
    resizeBytesIfNeeded();
    pushBackData(std::move(data));
  }

  void popBack() {
    assert(isValid());
    assert(!isEmpty());

    ValueType *back = bytes + (size_ - 1);
    std::allocator_traits<AllocatorType>::destroy(allocator, back);
    --size_;
  }

  void unorderedRemove(std::size_t index) {
    assert(isValid());
    assert(!isEmpty());
    assert(index < size_);

    if (index == size_ - 1) {
      popBack();
      return;
    }

    ValueType *target = bytes + index;
    std::allocator_traits<AllocatorType>::destroy(allocator, target);
    ValueType *back = bytes + (size_ - 1);
    std::memcpy(target, back, sizeof(ValueType));
    --size_;
  }
};
}  // namespace tlo

#endif  // TLO_DARRAY_HPP
