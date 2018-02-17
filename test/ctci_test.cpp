#include <tlo/ctci.hpp>
#include <tlo/test.hpp>

namespace {
TLO_TEST(IsUnique) {
  TLO_EXPECT(tlo::isUnique("a"));
  TLO_EXPECT(tlo::isUnique("ab"));
  TLO_EXPECT(tlo::isUnique("abc"));
  TLO_EXPECT(tlo::isUnique("abcd"));

  TLO_EXPECT(!tlo::isUnique("aa"));
  TLO_EXPECT(!tlo::isUnique("abb"));
  TLO_EXPECT(!tlo::isUnique("abcc"));
  TLO_EXPECT(!tlo::isUnique("abcdd"));
}

TLO_TEST(CheckPermutation) {
  TLO_EXPECT(tlo::checkPermutation("a", "a"));

  TLO_EXPECT(tlo::checkPermutation("ab", "ab"));
  TLO_EXPECT(tlo::checkPermutation("ab", "ba"));

  TLO_EXPECT(tlo::checkPermutation("abc", "abc"));
  TLO_EXPECT(tlo::checkPermutation("abc", "acb"));
  TLO_EXPECT(tlo::checkPermutation("abc", "bac"));
  TLO_EXPECT(tlo::checkPermutation("abc", "bca"));
  TLO_EXPECT(tlo::checkPermutation("abc", "cab"));
  TLO_EXPECT(tlo::checkPermutation("abc", "cba"));

  TLO_EXPECT(!tlo::checkPermutation("a", "b"));
  TLO_EXPECT(!tlo::checkPermutation("ab", "bc"));
  TLO_EXPECT(!tlo::checkPermutation("abc", "bcd"));

  TLO_EXPECT(!tlo::checkPermutation("a", "aa"));
  TLO_EXPECT(!tlo::checkPermutation("ab", "abb"));
  TLO_EXPECT(!tlo::checkPermutation("abc", "abcc"));
}

TLO_TEST(Urlify) {
  std::string string = "   ";
  tlo::urlify(string, 1);
  TLO_EXPECT(string == "%20");

  string = "      ";
  tlo::urlify(string, 2);
  TLO_EXPECT(string == "%20%20");

  string = "a";
  tlo::urlify(string, 1);
  TLO_EXPECT(string == "a");

  string = "ab";
  tlo::urlify(string, 2);
  TLO_EXPECT(string == "ab");

  string = "Mr John Smith    ";
  tlo::urlify(string, 13);
  TLO_EXPECT(string == "Mr%20John%20Smith");
}

TLO_TEST(PalindromePermutation) {
  TLO_EXPECT(tlo::palindromePermutation("abccba"));
  TLO_EXPECT(tlo::palindromePermutation("abcdcba"));
  TLO_EXPECT(tlo::palindromePermutation("abcdddcba"));
  TLO_EXPECT(tlo::palindromePermutation("dabcdcbad"));
  TLO_EXPECT(tlo::palindromePermutation("Tact Coa"));

  TLO_EXPECT(tlo::palindromePermutationWithBitVector("abccba"));
  TLO_EXPECT(tlo::palindromePermutationWithBitVector("abcdcba"));
  TLO_EXPECT(tlo::palindromePermutationWithBitVector("abcdddcba"));
  TLO_EXPECT(tlo::palindromePermutationWithBitVector("dabcdcbad"));
  TLO_EXPECT(tlo::palindromePermutationWithBitVector("Tact Coa"));
}
}  // namespace
