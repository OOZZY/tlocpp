#ifndef TLO_TEST_HPP
#define TLO_TEST_HPP

#include <deque>

namespace tlo {
namespace test {
struct Test {
  virtual const char *name() const = 0;
  virtual void run() const = 0;
  virtual ~Test();
};

std::deque<const Test *> &constructOrGetTests();

#define TLO_TEST(TestName)                                                   \
  struct Test##TestName : ::tlo::test::Test {                                \
    Test##TestName() { ::tlo::test::constructOrGetTests().push_back(this); } \
    const char *name() const override { return #TestName; }                  \
    void run() const override;                                               \
  };                                                                         \
  const Test##TestName test##TestName;                                       \
  void Test##TestName::run() const

void expect(bool isExpect, bool condition, const char *file, int line,
            const char *func, const char *conditionString);

#define TLO_EXPECT(condition)                                          \
  ::tlo::test::expect(true, (condition), __FILE__, __LINE__, __func__, \
                      #condition)

#define TLO_ASSERT(condition)                                             \
  do {                                                                    \
    ::tlo::test::expect(false, (condition), __FILE__, __LINE__, __func__, \
                        #condition);                                      \
    if (!(condition)) {                                                   \
      return;                                                             \
    }                                                                     \
  } while (0)

void runTests();
void printReport();
[[noreturn]] void exit();
}  // namespace test
}  // namespace tlo

#endif  // TLO_TEST_HPP
