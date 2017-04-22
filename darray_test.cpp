#include <iostream>
#include <tlo/darray.hpp>
#include <tlo/test.hpp>
#include <tlo/util.hpp>
#include "tlocpp_test.hpp"

#define DECLTYPE(thing) std::remove_reference<decltype(thing)>::type

#define EXPECT_DARRAY_PROPERTIES(darray, size_, isEmpty_, valueType,      \
                                 allocatorType)                           \
  do {                                                                    \
    TLO_EXPECT((darray).size() == (size_));                               \
    TLO_EXPECT((darray).capacity() >= (darray).size());                   \
    TLO_EXPECT((darray).isEmpty() == (isEmpty_));                         \
    TLO_EXPECT(typeid(DECLTYPE(darray)::ValueType) == typeid(valueType)); \
    TLO_EXPECT(typeid(DECLTYPE(darray)::AllocatorType) ==                 \
               typeid(allocatorType));                                    \
  } while (0)

namespace {
using DArrayInt = tlo::DArray<int, tlo::CountingAllocator<int>>;
using DArrayIntPtr =
    tlo::DArray<tlo::IntPtr, tlo::CountingAllocator<tlo::IntPtr>>;

void testDArrayIntConstructDestruct() {
  DArrayInt ints;
  EXPECT_DARRAY_PROPERTIES(ints, 0, true, int, tlo::CountingAllocator<int>);
}

#define EXPECT_DARRAY_ALL_PROPERTIES(darray, size, capacity_, isEmpty,         \
                                     valueType, allocatorType)                 \
  do {                                                                         \
    EXPECT_DARRAY_PROPERTIES(darray, size, isEmpty, valueType, allocatorType); \
    TLO_EXPECT((darray).capacity() == (capacity_));                            \
  } while (0)

void testDArrayIntConstructWithCapacityDestruct() {
  DArrayInt ints(SOME_NUMBER);
  EXPECT_DARRAY_ALL_PROPERTIES(ints, 0, SOME_NUMBER, true, int,
                               tlo::CountingAllocator<int>);
}

void testDArrayIntNewDelete() {
  auto ints = new DArrayInt;
  // "decltype(*ints)" is "tlo::DArray<int, tlo::CountingAllocator<int>> &"
  // "DECLTYPE(*ints)" is "tlo::DArray<int, tlo::CountingAllocator<int>>"
  EXPECT_DARRAY_PROPERTIES(*ints, 0, true, int, tlo::CountingAllocator<int>);
  delete ints;
  ints = nullptr;
}

void testDArrayIntNewWithCapacityDelete() {
  auto ints = new DArrayInt(SOME_NUMBER);
  EXPECT_DARRAY_ALL_PROPERTIES(*ints, 0, SOME_NUMBER, true, int,
                               tlo::CountingAllocator<int>);
  delete ints;
  ints = nullptr;
}

#define EXPECT_DARRAY_INT_ELEMENTS(darray, index, indexValue, frontValue,      \
                                   backValue)                                  \
  do {                                                                         \
    TLO_EXPECT(static_cast<const DArrayInt &>(darray)[index] == (indexValue)); \
    TLO_EXPECT((darray)[index] == (indexValue));                               \
    TLO_EXPECT(static_cast<const DArrayInt &>(darray).front() ==               \
               (frontValue));                                                  \
    TLO_EXPECT((darray).front() == (frontValue));                              \
    TLO_EXPECT(static_cast<const DArrayInt &>(darray).back() == (backValue));  \
    TLO_EXPECT((darray).back() == (backValue));                                \
  } while (0)

void testDArrayIntPushOrMoveBackOnce(bool testPush) {
  DArrayInt ints;
  int value = SOME_NUMBER;
  if (testPush) {
    ints.pushBack(value);
  } else {
    ints.pushBack(std::move(value));
  }
  EXPECT_DARRAY_PROPERTIES(ints, 1, false, int, tlo::CountingAllocator<int>);
  EXPECT_DARRAY_INT_ELEMENTS(ints, 0, SOME_NUMBER, SOME_NUMBER, SOME_NUMBER);
}

void testDArrayIntPushOrMoveBackUntilResize(bool testPush) {
  DArrayInt ints;
  for (std::size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = static_cast<int>(i);
    if (testPush) {
      ints.pushBack(value);
    } else {
      ints.pushBack(std::move(value));
    }
    EXPECT_DARRAY_PROPERTIES(ints, i + 1, false, int,
                             tlo::CountingAllocator<int>);
    EXPECT_DARRAY_INT_ELEMENTS(ints, i, static_cast<int>(i), 0,
                               static_cast<int>(i));
  }
}

void testDArrayIntPushBackOncePopBackOnce() {
  DArrayInt ints;
  int value = SOME_NUMBER;
  ints.pushBack(value);
  ints.popBack();
  EXPECT_DARRAY_PROPERTIES(ints, 0, true, int, tlo::CountingAllocator<int>);
}

void testDArrayIntPushBackUntilResizePopBackUntilEmpty() {
  DArrayInt ints;

  for (std::size_t i = 0; i < SOME_NUMBER; ++i) {
    int value = static_cast<int>(i);
    ints.pushBack(value);
  }

  for (std::size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(ints, i + 1, false, int,
                             tlo::CountingAllocator<int>);
    EXPECT_DARRAY_INT_ELEMENTS(ints, i, static_cast<int>(i), 0,
                               static_cast<int>(i));
    ints.popBack();
  }

  EXPECT_DARRAY_PROPERTIES(ints, 0, true, int, tlo::CountingAllocator<int>);
}

void testDArrayIntConstructCopy() {
  DArrayInt ints;

  for (int i = 0; i < SOME_NUMBER; ++i) {
    ints.pushBack(i);
  }

  DArrayInt copy(ints);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, copy.size(), copy.capacity(),
                               copy.isEmpty(), DECLTYPE(copy)::ValueType,
                               DECLTYPE(copy)::AllocatorType);

  for (std::size_t i = 0; i < ints.size(); ++i) {
    TLO_EXPECT(&ints[i] != &copy[i]);
    TLO_EXPECT(ints[i] == copy[i]);
  }
}

void testDArrayIntNewCopy() {
  DArrayInt ints;

  for (int i = 0; i < SOME_NUMBER; ++i) {
    ints.pushBack(i);
  }

  auto copy = new DArrayInt(ints);

  EXPECT_DARRAY_ALL_PROPERTIES(ints, copy->size(), copy->capacity(),
                               copy->isEmpty(), DECLTYPE(*copy)::ValueType,
                               DECLTYPE(*copy)::AllocatorType);

  for (std::size_t i = 0; i < ints.size(); ++i) {
    TLO_EXPECT(&ints[i] != &(*copy)[i]);
    TLO_EXPECT(ints[i] == (*copy)[i]);
  }

  delete copy;
  copy = nullptr;
}

void testDArrayIntCopy() {
  DArrayInt ints;

  for (int i = 0; i < SOME_NUMBER; ++i) {
    ints.pushBack(i);
  }

  DArrayInt copy;
  copy = ints;

  EXPECT_DARRAY_ALL_PROPERTIES(ints, copy.size(), copy.capacity(),
                               copy.isEmpty(), DECLTYPE(copy)::ValueType,
                               DECLTYPE(copy)::AllocatorType);

  for (std::size_t i = 0; i < ints.size(); ++i) {
    TLO_EXPECT(&ints[i] != &copy[i]);
    TLO_EXPECT(ints[i] == copy[i]);
  }
}

#define EXPECT_DARRAY_INTPTR_ELEMENTS(darray, index, indexValue, frontValue, \
                                      backValue)                             \
  do {                                                                       \
    TLO_EXPECT(*static_cast<const DArrayIntPtr &>(darray)[index].ptr ==      \
               (indexValue));                                                \
    TLO_EXPECT(*(darray)[index].ptr == (indexValue));                        \
    TLO_EXPECT(*static_cast<const DArrayIntPtr &>(darray).front().ptr ==     \
               (frontValue));                                                \
    TLO_EXPECT(*(darray).front().ptr == (frontValue));                       \
    TLO_EXPECT(*static_cast<const DArrayIntPtr &>(darray).back().ptr ==      \
               (backValue));                                                 \
    TLO_EXPECT(*(darray).back().ptr == (backValue));                         \
  } while (0)

void testDArrayIntPtrPushOrMoveBackOnce(bool testPush) {
  DArrayIntPtr intPtrs;
  tlo::IntPtr intPtr;
  *intPtr.ptr = SOME_NUMBER;
  if (testPush) {
    intPtrs.pushBack(intPtr);
  } else {
    intPtrs.pushBack(std::move(intPtr));
  }
  EXPECT_DARRAY_PROPERTIES(intPtrs, 1, false, tlo::IntPtr,
                           tlo::CountingAllocator<tlo::IntPtr>);
  EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, 0, SOME_NUMBER, SOME_NUMBER,
                                SOME_NUMBER);
}

void testDArrayIntPtrPushOrMoveBackUntilResize(bool testPush) {
  DArrayIntPtr intPtrs;
  for (std::size_t i = 0; i < SOME_NUMBER; ++i) {
    tlo::IntPtr intPtr;
    *intPtr.ptr = static_cast<int>(i);
    if (testPush) {
      intPtrs.pushBack(intPtr);
    } else {
      intPtrs.pushBack(std::move(intPtr));
    }
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, tlo::IntPtr,
                             tlo::CountingAllocator<tlo::IntPtr>);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, static_cast<int>(i), 0,
                                  static_cast<int>(i));
  }
}

void testDArrayIntPtrPushBackOncePopBackOnce() {
  DArrayIntPtr intPtrs;
  tlo::IntPtr intPtr;
  *intPtr.ptr = SOME_NUMBER;
  intPtrs.pushBack(intPtr);
  intPtrs.popBack();
  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, tlo::IntPtr,
                           tlo::CountingAllocator<tlo::IntPtr>);
}

void testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty() {
  DArrayIntPtr intPtrs;

  for (std::size_t i = 0; i < SOME_NUMBER; ++i) {
    tlo::IntPtr intPtr;
    *intPtr.ptr = static_cast<int>(i);
    intPtrs.pushBack(intPtr);
  }

  for (std::size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, tlo::IntPtr,
                             tlo::CountingAllocator<tlo::IntPtr>);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, static_cast<int>(i), 0,
                                  static_cast<int>(i));
    intPtrs.popBack();
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, tlo::IntPtr,
                           tlo::CountingAllocator<tlo::IntPtr>);
}

void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty() {
  DArrayIntPtr intPtrs;

  for (std::size_t i = 0; i < SOME_NUMBER; ++i) {
    tlo::IntPtr intPtr;
    *intPtr.ptr = static_cast<int>(i);
    intPtrs.pushBack(intPtr);
  }

  for (std::size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, tlo::IntPtr,
                             tlo::CountingAllocator<tlo::IntPtr>);
    EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, static_cast<int>(i), 0,
                                  static_cast<int>(i));
    intPtrs.unorderedRemove(intPtrs.size() - 1);
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, tlo::IntPtr,
                           tlo::CountingAllocator<tlo::IntPtr>);
}

void testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty() {
  DArrayIntPtr intPtrs;

  for (std::size_t i = 0; i < SOME_NUMBER; ++i) {
    tlo::IntPtr intPtr;
    *intPtr.ptr = static_cast<int>(i);
    intPtrs.pushBack(intPtr);
  }

  for (std::size_t i = SOME_NUMBER - 1; i <= SOME_NUMBER - 1; --i) {
    EXPECT_DARRAY_PROPERTIES(intPtrs, i + 1, false, tlo::IntPtr,
                             tlo::CountingAllocator<tlo::IntPtr>);

    if (i == SOME_NUMBER - 1) {
      EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, SOME_NUMBER - 1, 0,
                                    SOME_NUMBER - 1);
    } else if (i == 0) {
      EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, 1, 1, 1);
    } else {
      EXPECT_DARRAY_INTPTR_ELEMENTS(intPtrs, i, static_cast<int>(i),
                                    static_cast<int>(i) + 1,
                                    static_cast<int>(i));
    }

    intPtrs.unorderedRemove(0);
  }

  EXPECT_DARRAY_PROPERTIES(intPtrs, 0, true, tlo::IntPtr,
                           tlo::CountingAllocator<tlo::IntPtr>);
}
}  // namespace

void testDArray() {
  tlo::countingAllocatorResetCounts();
  TLO_EXPECT(tlo::countingAllocatorAllocateCount() == 0);
  TLO_EXPECT(tlo::countingAllocatorAllocateCount() ==
             tlo::countingAllocatorDeallocateCount());
  TLO_EXPECT(tlo::countingAllocatorTotalByteCount() == 0);

  testDArrayIntConstructDestruct();
  testDArrayIntConstructWithCapacityDestruct();
  testDArrayIntNewDelete();
  testDArrayIntNewWithCapacityDelete();
  testDArrayIntPushOrMoveBackOnce(true);
  testDArrayIntPushOrMoveBackOnce(false);
  testDArrayIntPushOrMoveBackUntilResize(true);
  testDArrayIntPushOrMoveBackUntilResize(false);
  testDArrayIntPushBackOncePopBackOnce();
  testDArrayIntPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntConstructCopy();
  testDArrayIntNewCopy();
  testDArrayIntCopy();
  testDArrayIntPtrPushOrMoveBackOnce(true);
  testDArrayIntPtrPushOrMoveBackOnce(false);
  testDArrayIntPtrPushOrMoveBackUntilResize(true);
  testDArrayIntPtrPushOrMoveBackUntilResize(false);
  testDArrayIntPtrPushBackOncePopBackOnce();
  testDArrayIntPtrPushBackUntilResizePopBackUntilEmpty();
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveBackUntilEmpty();
  testDArrayIntPtrPushBackUntilResizeUnorderedRemoveFrontUntilEmpty();

  TLO_EXPECT(tlo::countingAllocatorAllocateCount() > 0);
  TLO_EXPECT(tlo::countingAllocatorAllocateCount() ==
             tlo::countingAllocatorDeallocateCount());
  TLO_EXPECT(tlo::countingAllocatorTotalByteCount() > 0);

  std::cout << "sizeof(tlo::DArray<int, tlo::CountingAllocator<int>>): "
            << sizeof(tlo::DArray<int, tlo::CountingAllocator<int>>)
            << std::endl;
  tlo::countingAllocatorPrintCounts();
  std::cout << "==================" << std::endl;
  std::cout << "DArray tests done." << std::endl;
  std::cout << "==================" << std::endl;
}
