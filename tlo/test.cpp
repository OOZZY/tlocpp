#include "tlo/test.hpp"
#include <iostream>

namespace tlo {
namespace test {
/*
std::vector<const Test*> &tests() {
  static std::vector<const Test*> tests;
  return tests;
}
*/

namespace {
int numExpects = 0;
int numExpectsFailed = 0;
int numAsserts = 0;
int numAssertsFailed = 0;
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

void printReport() {
  std::cout << "=========================" << std::endl;
  std::cout << "# expects       : " << numExpects << std::endl;
  std::cout << "# expects failed: " << numExpectsFailed << std::endl;
  std::cout << "# asserts       : " << numAsserts << std::endl;
  std::cout << "# asserts failed: " << numAssertsFailed << std::endl;
  std::cout << "=========================" << std::endl;
}

void exit() {
  if (numExpectsFailed || numAssertsFailed) {
    std::exit(EXIT_FAILURE);
  }
  std::exit(EXIT_SUCCESS);
}
}  // namespace test
}  // namespace tlo
