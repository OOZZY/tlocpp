#include <memory>
#include <tlo/test.hpp>

namespace {
const int ANSWER = 42;

TLO_TEST(TestCase1) {
  std::unique_ptr<int> intPtr{std::make_unique<int>(ANSWER)};
  // use !! to avoid "no viable conversion from 'const std::unique_ptr<int>'
  // to 'bool'" error
  TLO_ASSERT(!!intPtr);
  TLO_EXPECT(*intPtr == ANSWER);
}

TLO_TEST(TestCase2) {
  std::unique_ptr<int> intPtr{std::make_unique<int>(ANSWER)};
  TLO_ASSERT(!!intPtr);
  TLO_EXPECT(*intPtr == ANSWER);
}

struct TestFixture1 : tlo::test::Test {
  std::unique_ptr<int> intPtr;

  // set up
  TestFixture1() : intPtr{std::make_unique<int>(ANSWER)} {}

  // tear down
  ~TestFixture1() {}
};

TLO_TEST_USING_FIXTURE(TestFixture1, TestCase1) {
  TLO_ASSERT(!!intPtr);
  TLO_EXPECT(*intPtr == ANSWER);
}

TLO_TEST_USING_FIXTURE(TestFixture1, TestCase2) {
  TLO_ASSERT(!!intPtr);
  TLO_EXPECT(*intPtr == ANSWER);
}

struct TestFixture2 : tlo::test::Test {
  int *intPtr;

  // set up
  TestFixture2() : intPtr{new int{ANSWER}} {}

  // tear down
  ~TestFixture2() { delete intPtr; }
};

TLO_TEST_USING_FIXTURE(TestFixture2, TestCase1) {
  TLO_ASSERT(intPtr);
  TLO_EXPECT(*intPtr == ANSWER);
}

TLO_TEST_USING_FIXTURE(TestFixture2, TestCase2) {
  TLO_ASSERT(intPtr);
  TLO_EXPECT(*intPtr == ANSWER);
}
}  // namespace
