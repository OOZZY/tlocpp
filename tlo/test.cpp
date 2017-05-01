#include "tlo/test.hpp"
#include <iostream>

namespace tlo {
namespace test {
Test::~Test() {}

std::deque<const Test *> &constructOrGetTests() {
  static std::deque<const Test *> tests;
  return tests;
}

namespace {
int numExpects = 0;
int numExpectsFailed = 0;
int numAsserts = 0;
int numAssertsFailed = 0;
int numObjectsCaught = 0;
}  // namespace

void expect(bool isExpect, bool condition, const char *file, int line,
            const char *func, const char *conditionString) {
  if (isExpect) {
    ++numExpects;
  } else {
    ++numAsserts;
  }

  if (!condition) {
    if (isExpect) {
      ++numExpectsFailed;
      std::cout << file << ":" << line << ": " << func
                << ": expect failed: " << conditionString << "." << std::endl;
    } else {
      ++numAssertsFailed;
      std::cout << file << ":" << line << ": " << func
                << ": assert failed: " << conditionString << "." << std::endl;
    }
  }
}

void runTests() {
  const std::deque<const Test *> &tests = constructOrGetTests();
  for (const Test *test : tests) {
    try {
      std::cout << "[ RUNNING ] " << test->testName() << std::endl;
      test->run();
      std::cout << "[ DONE    ] " << test->testName() << std::endl;
    } catch (const std::exception &e) {
      ++numObjectsCaught;
      std::cout << e.what() << std::endl;
      std::cout << "[ ERROR   ] " << test->testName() << " threw an exception"
                << std::endl;
    } catch (...) {
      ++numObjectsCaught;
      std::cout << "[ ERROR   ] " << test->testName() << " threw an object"
                << std::endl;
    }
  }
}

void printReport() {
  std::cout << "=========================" << std::endl;
  std::cout << "# expects       : " << numExpects << std::endl;
  std::cout << "# expects failed: " << numExpectsFailed << std::endl;
  std::cout << "# asserts       : " << numAsserts << std::endl;
  std::cout << "# asserts failed: " << numAssertsFailed << std::endl;
  std::cout << "# objects caught: " << numObjectsCaught << std::endl;
  std::cout << "=========================" << std::endl;
}

void exit() {
  if (numExpectsFailed || numAssertsFailed || numObjectsCaught) {
    std::cout << "[ FAILED  ]" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::cout << "[ PASSED  ]" << std::endl;
  std::exit(EXIT_SUCCESS);
}
}  // namespace test
}  // namespace tlo
