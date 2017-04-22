#include "tlo/bigint.hpp"
#include <cassert>
#include <cctype>
#include "tlo/util.hpp"

#include <iostream>

namespace tlo {
namespace {
const UBigInt::Digit BASE = 100;
const UBigInt::Digit BASE_MAX = 99;

// returns whether base10Int is a properly formatted base 10 integer
bool isBase10Int(const std::string &base10Int) {
  std::size_t i = 0;

  if (base10Int[0] == '+' || base10Int[0] == '-') {
    i++;
  }

  if (i == base10Int.size()) {  // if base10Int is empty (excluding +/-)
    return false;
  }

  for (; i < base10Int.size(); ++i) {
    if (!std::isdigit(base10Int[i])) {
      return false;
    }
  }

  return true;
}

// - returns the number of base 10 digits in base10Int
// - assumes isBase10Int(base10Int)
std::size_t numBase10Digits(const std::string &base10Int) {
  if (!base10Int.empty() && (base10Int[0] == '+' || base10Int[0] == '-')) {
    return base10Int.size() - 1;
  }
  return base10Int.size();
}

// - returns the number of base 100 digits required to repesent the given
//   number of base 10 digits
// - if numBase10Digits == 0, then returns 0
// - if numBase10Digits == 1, then returns 1
// - if numBase10Digits == 2, then returns 1
// - if numBase10Digits == 3, then returns 2
// - if numBase10Digits == 4, then returns 2
// - if numBase10Digits == 5, then returns 3
// - if numBase10Digits == 6, then returns 3
// - etc.
std::size_t numBase100DigitsRequired(std::size_t numBase10Digits) {
  return (numBase10Digits + 1) / 2;
}

// since base100Int's digits are in little endian order, leadings 0s are at the
// end
void removeLeading0s(UBigInt::Data &base100Int) {
  while (base100Int.size() > 1 && base100Int.back() == 0) {
    base100Int.pop_back();
  }
}

// convert base10Int to base 100 digits in little endian order
UBigInt::Data toBase100(const std::string &base10Int) {
  if (!isBase10Int(base10Int)) {
    return UBigInt::Data{0};
  }

  std::size_t numBase100Digits =
      numBase100DigitsRequired(numBase10Digits(base10Int));
  UBigInt::Data base100Int(numBase100Digits, 0);

  for (std::size_t i = 0; i < base10Int.size(); ++i) {
    unsigned base10Power = i % 2;
    unsigned base10Digit = static_cast<unsigned>(tlo::rat(base10Int, i) - '0');
    unsigned base10Value = base10Digit;

    if (base10Power == 1) {
      base10Value *= 10;
    }

    std::size_t base100Index = i / 2;
    base100Int[base100Index] += base10Value;
  }

  removeLeading0s(base100Int);
  return base100Int;
}
}  // namespace

UBigInt::UBigInt(const std::string &base10Int)
    : base100Int(toBase100(base10Int)) {}

UBigInt::UBigInt(const char *base10Int) : UBigInt(std::string(base10Int)) {}

UBigInt::UBigInt(const UBigInt::Data &base100Int_) : base100Int(base100Int_) {
  for (Digit &digit : base100Int) {
    digit %= BASE;
  }
  removeLeading0s(base100Int);
}

namespace {
// convert base10Int to base 100 digits in little endian order
UBigInt::Data toBase100(unsigned long long base10Int) {
  if (base10Int == 0) {
    return UBigInt::Data{0};
  }

  UBigInt::Data digits;
  while (base10Int > 0) {
    digits.push_back(base10Int % BASE);
    base10Int /= BASE;
  }
  return digits;
}
}  // namespace

UBigInt::UBigInt(unsigned long long base10Int)
    : base100Int(toBase100(base10Int)) {}

UBigInt::UBigInt() { base100Int.push_back(0); }

UBigInt::UBigInt(std::size_t numBase100Digits, bool disambiguator)
    : base100Int(numBase100Digits, 0) {
  static_cast<void>(disambiguator);
}

const UBigInt::Data &UBigInt::data() const { return base100Int; }

int UBigInt::compareTo(const UBigInt &other) const {
  if (base100Int.size() < other.base100Int.size()) {
    return -1;
  } else if (base100Int.size() > other.base100Int.size()) {
    return 1;
  }
  for (std::size_t i = 0; i < base100Int.size(); ++i) {
    Digit thisDigit = tlo::rat(base100Int, i);
    Digit otherDigit = tlo::rat(other.base100Int, i);
    if (thisDigit != otherDigit) {
      return thisDigit - otherDigit;
    }
  }
  return 0;
}

bool UBigInt::operator<(const UBigInt &other) const {
  return compareTo(other) < 0;
}

bool UBigInt::operator<=(const UBigInt &other) const {
  return compareTo(other) <= 0;
}

bool UBigInt::operator>(const UBigInt &other) const {
  return compareTo(other) > 0;
}

bool UBigInt::operator>=(const UBigInt &other) const {
  return compareTo(other) >= 0;
}

bool UBigInt::operator==(const UBigInt &other) const {
  return compareTo(other) == 0;
}

bool UBigInt::operator!=(const UBigInt &other) const {
  return compareTo(other) != 0;
}

namespace {
// - push back 0s to base100Int until base100Int.size() > size
// - does nothing if base100Int.size() > size is already true
void makeSizeGreaterThan(UBigInt::Data &base100Int, std::size_t size) {
  std::size_t targetSize = size + 1;
  while (base100Int.size() < targetSize) {
    base100Int.push_back(0);
  }
}
}  // namespace

bool UBigInt::is(UBigInt::Digit digit) const {
  return base100Int.size() == 1 && base100Int[0] == digit;
}

UBigInt &UBigInt::add(const UBigInt &other) {
  if (other.is(0)) {
    return *this;
  }

  if (is(0)) {
    *this = other;
    return *this;
  }

  makeSizeGreaterThan(base100Int, other.base100Int.size());

  Digit carryOver = 0;  // will be at most 1
  for (std::size_t i = 0; i < base100Int.size(); ++i) {
    Digit otherDigit = other.base100Int[i];

    if (i >= other.base100Int.size()) {
      otherDigit = 0;
    }

    base100Int[i] += otherDigit + carryOver;
    if (base100Int[i] > BASE_MAX) {
      base100Int[i] -= BASE;
      carryOver = 1;
    } else {
      carryOver = 0;
    }
  }

  removeLeading0s(base100Int);
  return *this;
}

UBigInt &UBigInt::operator+=(const UBigInt &other) { return add(other); }

UBigInt UBigInt::operator+(const UBigInt &other) const {
  UBigInt temp(*this);
  temp += other;
  return temp;
}

UBigInt &UBigInt::subtract(const UBigInt &other) {
  if (is(0) || other.is(0)) {
    return *this;
  }

  if (compareTo(other) >= 0) {
    Digit borrow = 0;  // will be at most 1
    for (std::size_t i = 0; i < base100Int.size(); ++i) {
      Digit otherDigit = other.base100Int[i];

      if (i >= other.base100Int.size()) {
        otherDigit = 0;
      }

      // using signed short because base100Int[i] is unsigned and might be 0
      signed short thisDigit = base100Int[i];
      thisDigit -= borrow;
      if (thisDigit < otherDigit) {
        thisDigit += BASE;
        borrow = 1;
      } else {
        borrow = 0;
      }
      thisDigit -= otherDigit;
      base100Int[i] = static_cast<Digit>(thisDigit);
    }
  }

  removeLeading0s(base100Int);
  return *this;
}

UBigInt &UBigInt::operator-=(const UBigInt &other) { return subtract(other); }

UBigInt UBigInt::operator-(const UBigInt &other) const {
  UBigInt temp(*this);
  temp -= other;
  return temp;
}

UBigInt &UBigInt::multiplyBy(UBigInt::Digit digit) {
  assert(digit < BASE);

  base100Int.push_back(0);

  Digit carryOver = 0;  // will be at most 98
  for (std::size_t i = 0; i < base100Int.size(); ++i) {
    unsigned short thisDigit = base100Int[i];
    thisDigit *= digit;
    thisDigit += carryOver;
    if (thisDigit > BASE_MAX) {
      carryOver = static_cast<Digit>(thisDigit / BASE);
      thisDigit %= BASE;
    } else {
      carryOver = 0;
    }
    base100Int[i] = static_cast<Digit>(thisDigit);
  }

  removeLeading0s(base100Int);
  return *this;
}

UBigInt &UBigInt::multiplyBy100() {
  base100Int.push_front(0);
  return *this;
}

void UBigInt::setTo(UBigInt::Digit digit) {
  base100Int.clear();
  base100Int.push_back(digit);
}

UBigInt &UBigInt::multiplyBy(const UBigInt &other) {
  if (is(0) || other.is(1)) {
    return *this;
  }

  if (other.is(0)) {
    setTo(0);
    return *this;
  }

  if (is(1)) {
    *this = other;
    return *this;
  }

  std::deque<UBigInt> products(other.base100Int.size(), base100Int);

  for (std::size_t i = 0; i < products.size(); ++i) {
    products[i].multiplyBy(other.base100Int[i]);

    for (std::size_t j = 0; j < i; ++j) {
      products[i].multiplyBy100();
    }
  }

  *this = products[0];
  for (std::size_t i = 1; i < products.size(); ++i) {
    add(products[i]);
  }

  return *this;
}

UBigInt &UBigInt::operator*=(const UBigInt &other) { return multiplyBy(other); }

UBigInt UBigInt::operator*(const UBigInt &other) const {
  UBigInt temp(*this);
  temp *= other;
  return temp;
}

UBigInt &UBigInt::divideBy_(const UBigInt &other, UBigInt &product) {
  // std::cout << *this << " / " << other << std::endl;

  Digit min = 0;
  Digit max = BASE_MAX;

  while (true) {
    // - min was chosen because guesses min-1 and min were both wrong and both
    //   too small
    // - max was chosen because guesses max and max+1 were both wrong and both
    //   too big
    // - min was too small and max was too big
    //   - doesn't make sense if min > max
    //   - doesn't make sense if min == max
    assert(min <= max);
    assert(min != max);

    Digit guess1 = (min + max) / 2;
    Digit guess2 = guess1 + 1;

    UBigInt product1 = other;
    product1.multiplyBy(guess1);
    int diff1 = compareTo(product1);

    UBigInt product2 = other;
    product2.multiplyBy(guess2);
    int diff2 = compareTo(product2);

    // std::cout << unsigned(min) << " to " << unsigned(max) << std::endl;
    // std::cout << "guesses: " << unsigned(guess1) << " and " <<
    //  unsigned(guess2) << std::endl;
    // std::cout << "products: " << product1 << " and " << product2 <<
    // std::endl;
    // std::cout << "diffs: " << diff1 << " and " << diff2 << std::endl;

    // if guess1 is correct
    // or if guess1 is too small and guess2 is too big
    if (diff1 == 0 || (diff1 > 0 && diff2 < 0)) {
      setTo(guess1);
      product = product1;
      break;
    }
    // if guess2 is correct
    if (diff2 == 0) {
      setTo(guess2);
      product = product2;
      break;
    }

    // at this point, guess1 is wrong, guess2 is wrong, and they are both too
    // small or both too big
    if (diff1 < 0) {
      max = guess1;
    } else {
      min = guess2;
    }
  }

  return *this;
}

UBigInt &UBigInt::divideBy_(const UBigInt &other, bool division) {
  if (is(0) || other.is(0)) {
    return *this;
  }

  if (division) {
    if (other.is(1)) {
      return *this;
    }
  } else {
    if (other.is(1)) {
      setTo(0);
      return *this;
    }

    if (is(1)) {
      setTo(1);
      return *this;
    }
  }

  if (compareTo(other) < 0) {
    if (division) {
      setTo(0);
    }
    return *this;
  }

  UBigInt quotient(0, true);
  UBigInt smallDividend(0, true);

  for (std::size_t i = 0; i < base100Int.size(); ++i) {
    if (smallDividend.is(0)) {
      smallDividend.base100Int.clear();
    }
    smallDividend.base100Int.push_front(tlo::rat(base100Int, i));

    // if (other.compareTo(smallDividend) <= 0) {
    UBigInt smallQuotient = smallDividend;
    UBigInt smallProduct;
    smallQuotient.divideBy_(other, smallProduct);

    quotient.base100Int.push_front(smallQuotient.base100Int[0]);
    smallDividend.subtract(smallProduct);
    //}
  }

  removeLeading0s(quotient.base100Int);

  if (division) {
    *this = quotient;
  } else {
    *this = smallDividend;
  }
  return *this;
}

UBigInt &UBigInt::divideBy100() {
  base100Int.pop_front();
  if (base100Int.empty()) {
    base100Int.push_back(0);
  }
  return *this;
}

UBigInt &UBigInt::divideBy(const UBigInt &other) {
  return divideBy_(other, true);
}

UBigInt &UBigInt::operator/=(const UBigInt &other) { return divideBy(other); }

UBigInt UBigInt::operator/(const UBigInt &other) const {
  UBigInt temp(*this);
  temp /= other;
  return temp;
}

UBigInt &UBigInt::moduloBy(const UBigInt &other) {
  return divideBy_(other, false);
}

UBigInt &UBigInt::operator%=(const UBigInt &other) { return moduloBy(other); }

UBigInt UBigInt::operator%(const UBigInt &other) const {
  UBigInt temp(*this);
  temp %= other;
  return temp;
}

UBigInt UBigInt::factorial() const {
  UBigInt product = 1;
  for (UBigInt i = 2; i <= *this; i += 1) {
    product *= i;
  }
  return product;
}

std::ostream &operator<<(std::ostream &os, const UBigInt &uBigInt) {
  for (std::size_t i = 0; i < uBigInt.base100Int.size(); ++i) {
    unsigned base100Digit =
        static_cast<unsigned>(tlo::rat(uBigInt.base100Int, i));
    if (i != 0 && base100Digit < 10) {
      os << 0;
    }
    os << base100Digit;
  }
  return os;
}
}  // namespace tlo
