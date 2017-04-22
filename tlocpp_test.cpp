#include "tlocpp_test.hpp"
#include <iostream>
#include <tlo/test.hpp>

int main() {
  testDArray();
  testCTCI();
  testBigInt();
  std::cout << "===============" << std::endl;
  std::cout << "All tests done." << std::endl;
  std::cout << "===============" << std::endl;
  tlo::test::printReport();
  tlo::test::exit();
}
