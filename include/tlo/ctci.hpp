#ifndef TLO_CTCI_HPP
#define TLO_CTCI_HPP

#include <string>

namespace tlo {
/*
 * returns whether the given string has only unique characters.
 */
bool isUnique(const std::string &string);

/*
 * returns whether the given strings are permutations of each other
 */
bool checkPermutation(const std::string &string1, const std::string &string2);

/*
 * - replaces all spaces in the given string with "%20"
 * - assumes that the string has sufficient space at the end to hold the
 *   additional characters
 * - assumes that the "true" length of the string is given
 */
void urlify(std::string &string, std::size_t trueLength);

/*
 * - returns whether the given string is a permutation of a palindrome
 * - ignores non-alphabetic characters
 * - case-insensitive
 */
bool palindromePermutation(const std::string &string);
bool palindromePermutationWithBitVector(const std::string &string);
}  // namespace tlo

#endif  // TLO_CTCI_HPP
