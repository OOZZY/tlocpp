#ifndef TLO_BIGINT_HPP
#define TLO_BIGINT_HPP

#include <deque>
#include <ostream>
#include <string>

namespace tlo {
class UBigInt {
 public:
  using Digit = unsigned char;
  using Data = std::deque<Digit>;

 private:
  Data base100Int;

 public:
  // - will ignore a leading '+' or '-'
  // - if base10Int is empty or contains only a '+' or a '-', does the same
  //   thing as UBigInt("0")
  UBigInt(const std::string &base10Int);
  UBigInt(const char *base10Int);

  // - assumes base100Int_ is in little endian order
  // - will %= 100 each element
  // - will remove trailing 0s (which represent leading 0s)
  UBigInt(const Data &base100Int_);

  UBigInt(unsigned long long base10Int);
  UBigInt();

 private:
  UBigInt(std::size_t numBase100Digits, bool disambiguator);

 public:
  const Data &data() const;

  int compareTo(const UBigInt &other) const;
  bool operator<(const UBigInt &other) const;
  bool operator<=(const UBigInt &other) const;
  bool operator>(const UBigInt &other) const;
  bool operator>=(const UBigInt &other) const;
  bool operator==(const UBigInt &other) const;
  bool operator!=(const UBigInt &other) const;

 private:
  // assumes digit < 100
  bool is(Digit digit) const;

 public:
  // elementary school addition
  UBigInt &add(const UBigInt &other);
  UBigInt &operator+=(const UBigInt &other);
  UBigInt operator+(const UBigInt &other) const;

  // - elementary school subtraction
  // - does nothing if *this < other
  UBigInt &subtract(const UBigInt &other);
  UBigInt &operator-=(const UBigInt &other);
  UBigInt operator-(const UBigInt &other) const;

 private:
  // assumes digit < 100
  UBigInt &multiplyBy(Digit digit);

  UBigInt &multiplyBy100();

  // assumes digit < 100
  void setTo(Digit digit);

 public:
  // elementary school multiplication
  UBigInt &multiplyBy(const UBigInt &other);
  UBigInt &operator*=(const UBigInt &other);
  UBigInt operator*(const UBigInt &other) const;

 private:
  // - assumes result of integer division is in [0, 99]
  // - sets product to other * (result of integer division)
  UBigInt &divideBy_(const UBigInt &other, UBigInt &product);

  UBigInt &divideBy_(const UBigInt &other, bool division);
  UBigInt &divideBy100();

 public:
  // - elementary school division
  // - does nothing if other == 0
  UBigInt &divideBy(const UBigInt &other);
  UBigInt &operator/=(const UBigInt &other);
  UBigInt operator/(const UBigInt &other) const;

  // remainder after elementary school division
  UBigInt &moduloBy(const UBigInt &other);
  UBigInt &operator%=(const UBigInt &other);
  UBigInt operator%(const UBigInt &other) const;

  UBigInt factorial() const;

  friend std::ostream &operator<<(std::ostream &os, const UBigInt &uBigInt);
};
}  // namespace tlo

#endif  // TLO_BIGINT_HPP
