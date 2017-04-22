#include "tlo/ctci.hpp"
#include <cctype>

namespace tlo {
namespace {
const int NUM_CHAR_VALUES = 256;
}  // namespace tlo

bool isUnique(const std::string &string) {
  if (string.size() > NUM_CHAR_VALUES) {
    return false;
  }

  bool occurs[NUM_CHAR_VALUES] = {false};
  for (const auto &ch : string) {
    const auto uch = static_cast<unsigned char>(ch);
    if (occurs[uch]) {
      return false;
    }
    occurs[uch] = true;
  }
  return true;
}

bool checkPermutation(const std::string &string1, const std::string &string2) {
  if (string1.size() != string2.size()) {
    return false;
  }

  int numOccurrences[NUM_CHAR_VALUES] = {0};
  for (const auto &ch : string1) {
    const auto uch = static_cast<unsigned char>(ch);
    numOccurrences[uch]++;
  }

  for (const auto &ch : string2) {
    const auto uch = static_cast<unsigned char>(ch);
    numOccurrences[uch]--;
    // if character has more occurrences in string2 than in string1
    if (numOccurrences[uch] < 0) {
      return false;
    }
  }

  return true;
}

void urlify(std::string &string, std::size_t trueLength) {
  std::size_t spaceIndex = string.size() - 1;

  for (std::size_t i = trueLength - 1; i < trueLength; --i) {
    if (string[i] != ' ') {
      string[spaceIndex] = string[i];
    } else {
      string[spaceIndex] = '0';
      spaceIndex--;
      string[spaceIndex] = '2';
      spaceIndex--;
      string[spaceIndex] = '%';
    }

    spaceIndex--;
  }
}

namespace {
bool noneTrue(const bool *bools, std::size_t size) {
  for (std::size_t i = 0; i < size; ++i) {
    if (bools[i]) {
      return false;
    }
  }
  return true;
}

bool oneTrue(const bool *bools, std::size_t size) {
  bool trueFound = false;

  for (std::size_t i = 0; i < size; ++i) {
    if (bools[i]) {
      if (!trueFound) {
        trueFound = true;
      } else {
        return false;
      }
    }
  }

  return trueFound;
}
}  // namespace tlo

/*
 * - if no character has an odd number of occurrences and every character has
 *   an even number of occurrences, the string is a palindrome
 *   - in this case, the palindrome has no middle character
 *   - example: abccba
 * - if one character has an odd number of occurrences and every other
 *   character has an even number of occurrences, the string is a palindrome
 *   - in this case, the one character with an odd number of occurrences is the
 *     middle character of the palindrome
 *   - example: abcdcba
 *   - example: abcdddcba
 *   - example: dabcdcbad
 */
bool palindromePermutation(const std::string &string) {
  bool hasOddOccurences[NUM_CHAR_VALUES] = {false};
  for (const auto &ch : string) {
    if (std::isalpha(ch)) {
      const auto uch = static_cast<unsigned char>(std::tolower(ch));
      hasOddOccurences[uch] = !hasOddOccurences[uch];
    }
  }

  return noneTrue(hasOddOccurences, NUM_CHAR_VALUES) ||
         oneTrue(hasOddOccurences, NUM_CHAR_VALUES);
}

bool palindromePermutationWithBitVector(const std::string &string) {
  unsigned int hasOddOccurences = 0;

  for (const auto &ch : string) {
    if (std::isalpha(ch)) {
      const auto index = static_cast<std::size_t>(std::tolower(ch) - 'a');
      hasOddOccurences ^= (1 << index);
    }
  }

  return hasOddOccurences == 0 ||
         (hasOddOccurences != 0 &&
          (hasOddOccurences & (hasOddOccurences - 1)) == 0);
}
}  // namespace tlo
