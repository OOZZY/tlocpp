#include <tlo/bigint.hpp>
#include <tlo/test.hpp>
#include <tlo/util.hpp>

namespace {
using UBigInt = tlo::UBigInt;
using Data = tlo::UBigInt::Data;

TLO_TEST(testUBigIntFromBase10String) {
  TLO_EXPECT(UBigInt("").data() == Data{0});
  TLO_EXPECT(UBigInt("+").data() == Data{0});
  TLO_EXPECT(UBigInt("-").data() == Data{0});

  TLO_EXPECT(UBigInt("1").data() == Data{1});
  TLO_EXPECT(UBigInt("10").data() == Data{10});
  TLO_EXPECT((UBigInt("100").data() == Data{0, 1}));
  TLO_EXPECT((UBigInt("1000").data() == Data{0, 10}));
  TLO_EXPECT((UBigInt("10000").data() == Data{0, 0, 1}));
  TLO_EXPECT((UBigInt("100000").data() == Data{0, 0, 10}));

  TLO_EXPECT(UBigInt("2").data() == Data{2});
  TLO_EXPECT(UBigInt("16").data() == Data{16});
  TLO_EXPECT((UBigInt("128").data() == Data{28, 1}));
  TLO_EXPECT((UBigInt("1024").data() == Data{24, 10}));
  TLO_EXPECT((UBigInt("16384").data() == Data{84, 63, 1}));
  TLO_EXPECT((UBigInt("131072").data() == Data{72, 10, 13}));

  TLO_EXPECT((UBigInt("+1024").data() == Data{24, 10}));
  TLO_EXPECT((UBigInt("-1024").data() == Data{24, 10}));

  TLO_EXPECT(UBigInt("0").data() == Data{0});
  TLO_EXPECT(UBigInt("00").data() == Data{0});
  TLO_EXPECT(UBigInt("000").data() == Data{0});
  TLO_EXPECT(UBigInt("0000").data() == Data{0});
  TLO_EXPECT(UBigInt("0001").data() == Data{1});
}

TLO_TEST(testUBigIntFromBase100) {
  TLO_EXPECT(UBigInt(Data{1}).data() == Data{1});
  TLO_EXPECT(UBigInt(Data{10}).data() == Data{10});
  TLO_EXPECT((UBigInt(Data{0, 1}).data() == Data{0, 1}));
  TLO_EXPECT((UBigInt(Data{0, 10}).data() == Data{0, 10}));
  TLO_EXPECT((UBigInt(Data{0, 0, 1}).data() == Data{0, 0, 1}));
  TLO_EXPECT((UBigInt(Data{0, 0, 10}).data() == Data{0, 0, 10}));

  TLO_EXPECT(UBigInt(Data{2}).data() == Data{2});
  TLO_EXPECT(UBigInt(Data{16}).data() == Data{16});
  TLO_EXPECT((UBigInt(Data{28, 1}).data() == Data{28, 1}));
  TLO_EXPECT((UBigInt(Data{24, 10}).data() == Data{24, 10}));
  TLO_EXPECT((UBigInt(Data{84, 63, 1}).data() == Data{84, 63, 1}));
  TLO_EXPECT((UBigInt(Data{72, 10, 13}).data() == Data{72, 10, 13}));

  TLO_EXPECT((UBigInt(Data{24, 10}).data() == Data{24, 10}));

  TLO_EXPECT((UBigInt(Data{0}).data() == Data{0}));
  TLO_EXPECT((UBigInt(Data{0, 0}).data() == Data{0}));
  TLO_EXPECT((UBigInt(Data{0, 0, 0}).data() == Data{0}));
  TLO_EXPECT((UBigInt(Data{1, 0, 0}).data() == Data{1}));

  TLO_EXPECT((UBigInt(Data{128}).data() == Data{28}));
  TLO_EXPECT((UBigInt(Data{128, 128}).data() == Data{28, 28}));
  TLO_EXPECT((UBigInt(Data{128, 128, 128}).data() == Data{28, 28, 28}));
}

TLO_TEST(testUBigIntFromBase10UnsignedLongLong) {
  // UBigInt(0) will be ambiguous because it could match either
  // UBigInt(const char *) or UBigInt(unsigned long long) so use UBigInt(0ULL)
  // to explicitly use UBigInt(unsigned long long) constructol with 0 as the
  // argument
  TLO_EXPECT(UBigInt(0ULL).data() == Data{0});

  TLO_EXPECT(UBigInt(1).data() == Data{1});
  TLO_EXPECT(UBigInt(10).data() == Data{10});
  TLO_EXPECT((UBigInt(100).data() == Data{0, 1}));
  TLO_EXPECT((UBigInt(1000).data() == Data{0, 10}));
  TLO_EXPECT((UBigInt(10000).data() == Data{0, 0, 1}));
  TLO_EXPECT((UBigInt(100000).data() == Data{0, 0, 10}));

  TLO_EXPECT(UBigInt(2).data() == Data{2});
  TLO_EXPECT(UBigInt(16).data() == Data{16});
  TLO_EXPECT((UBigInt(128).data() == Data{28, 1}));
  TLO_EXPECT((UBigInt(1024).data() == Data{24, 10}));
  TLO_EXPECT((UBigInt(16384).data() == Data{84, 63, 1}));
  TLO_EXPECT((UBigInt(131072).data() == Data{72, 10, 13}));
}

TLO_TEST(testUBigIntCompareTo) {
  TLO_EXPECT(UBigInt("1") < "131072");
  TLO_EXPECT(UBigInt("131072") > ("1"));

  TLO_EXPECT(UBigInt("1") < "2");
  TLO_EXPECT(UBigInt("10") < "16");
  TLO_EXPECT(UBigInt("100") < "128");
  TLO_EXPECT(UBigInt("1000") < "1024");
  TLO_EXPECT(UBigInt("10000") < "16384");
  TLO_EXPECT(UBigInt("100000") < "131072");

  TLO_EXPECT(UBigInt("2") > "1");
  TLO_EXPECT(UBigInt("16") > "10");
  TLO_EXPECT(UBigInt("128") > "100");
  TLO_EXPECT(UBigInt("1024") > "1000");
  TLO_EXPECT(UBigInt("16384") > "10000");
  TLO_EXPECT(UBigInt("131072") > "100000");

  TLO_EXPECT(UBigInt("2") == "2");
  TLO_EXPECT(UBigInt("16") == "16");
  TLO_EXPECT(UBigInt("128") == "128");
  TLO_EXPECT(UBigInt("1024") == "1024");
  TLO_EXPECT(UBigInt("16384") == "16384");
  TLO_EXPECT(UBigInt("131072") == "131072");

  TLO_EXPECT(UBigInt("+1024") == "-1024");
}

TLO_TEST(testUBigIntAddition) {
  TLO_EXPECT(UBigInt("2") + "2" == "4");
  TLO_EXPECT(UBigInt("16") + "16" == "32");
  TLO_EXPECT(UBigInt("128") + "128" == "256");
  TLO_EXPECT(UBigInt("1024") + "1024" == "2048");
  TLO_EXPECT(UBigInt("16384") + "16384" == "32768");
  TLO_EXPECT(UBigInt("131072") + "131072" == "262144");

  TLO_EXPECT(UBigInt("131072") + "128" == "131200");
  TLO_EXPECT(UBigInt("128") + "131072" == "131200");

  TLO_EXPECT(UBigInt("131072") + "1024" == "132096");
  TLO_EXPECT(UBigInt("1024") + "131072" == "132096");

  TLO_EXPECT(UBigInt("0") + "0" == "0");
  TLO_EXPECT(UBigInt("131072") + "0" == "131072");
  TLO_EXPECT(UBigInt("0") + "131072" == "131072");

  TLO_EXPECT(UBigInt("1000000") + "1000000" == "2000000");
  TLO_EXPECT(UBigInt("1000000") + "999999" == "1999999");
  TLO_EXPECT(UBigInt("1000100") + "999999" == "2000099");

  TLO_EXPECT(UBigInt("1") + "1" == "2");
  TLO_EXPECT(UBigInt("131072") + "1" == "131073");
  TLO_EXPECT(UBigInt("1") + "131072" == "131073");

  TLO_EXPECT(UBigInt("42000000") + "42" == "42000042");

  // propogate carry
  TLO_EXPECT(UBigInt("42999999") + "42" == "43000041");
}

TLO_TEST(testUBigIntSubtraction) {
  TLO_EXPECT(UBigInt("2") - "2" == "0");
  TLO_EXPECT(UBigInt("16") - "16" == "0");
  TLO_EXPECT(UBigInt("128") - "128" == "0");
  TLO_EXPECT(UBigInt("1024") - "1024" == "0");
  TLO_EXPECT(UBigInt("16384") - "16384" == "0");
  TLO_EXPECT(UBigInt("131072") - "131072" == "0");

  TLO_EXPECT(UBigInt("131072") - "128" == "130944");
  TLO_EXPECT(UBigInt("128") - "131072" == "128");

  TLO_EXPECT(UBigInt("131072") - "1024" == "130048");
  TLO_EXPECT(UBigInt("1024") - "131072" == "1024");

  TLO_EXPECT(UBigInt("0") - "0" == "0");
  TLO_EXPECT(UBigInt("131072") - "0" == "131072");
  TLO_EXPECT(UBigInt("0") - "131072" == "0");

  TLO_EXPECT(UBigInt("1000000") - "1000000" == "0");
  TLO_EXPECT(UBigInt("1000000") - "999999" == "1");
  TLO_EXPECT(UBigInt("1000100") - "999999" == "101");

  TLO_EXPECT(UBigInt("1") - "1" == "0");
  TLO_EXPECT(UBigInt("131072") - "1" == "131071");
  TLO_EXPECT(UBigInt("1") - "131072" == "1");

  // propogate borrow
  TLO_EXPECT(UBigInt("42000000") - "42" == "41999958");

  TLO_EXPECT(UBigInt("42999999") - "42" == "42999957");
}

TLO_TEST(testUBigIntMultiplication) {
  TLO_EXPECT(UBigInt("2") * "2" == "4");
  TLO_EXPECT(UBigInt("16") * "16" == "256");
  TLO_EXPECT(UBigInt("128") * "128" == "16384");
  TLO_EXPECT(UBigInt("1024") * "1024" == "1048576");
  TLO_EXPECT(UBigInt("16384") * "16384" == "268435456");
  TLO_EXPECT(UBigInt("131072") * "131072" == "17179869184");

  TLO_EXPECT(UBigInt("131072") * "128" == "16777216");
  TLO_EXPECT(UBigInt("128") * "131072" == "16777216");

  TLO_EXPECT(UBigInt("131072") * "1024" == "134217728");
  TLO_EXPECT(UBigInt("1024") * "131072" == "134217728");

  TLO_EXPECT(UBigInt("0") * "0" == "0");
  TLO_EXPECT(UBigInt("131072") * "0" == "0");
  TLO_EXPECT(UBigInt("0") * "131072" == "0");

  TLO_EXPECT(UBigInt("1000000") * "1000000" == "1000000000000");
  TLO_EXPECT(UBigInt("1000000") * "999999" == "999999000000");
  TLO_EXPECT(UBigInt("1000100") * "999999" == "1000098999900");

  TLO_EXPECT(UBigInt("1") * "1" == "1");
  TLO_EXPECT(UBigInt("131072") * "1" == "131072");
  TLO_EXPECT(UBigInt("1") * "131072" == "131072");

  TLO_EXPECT(UBigInt("42000000") * "42" == "1764000000");

  TLO_EXPECT(UBigInt("42999999") * "42" == "1805999958");
}

TLO_TEST(testUBigIntDivision) {
  TLO_EXPECT(UBigInt("2") / "2" == "1");
  TLO_EXPECT(UBigInt("16") / "16" == "1");
  TLO_EXPECT(UBigInt("128") / "128" == "1");
  TLO_EXPECT(UBigInt("1024") / "1024" == "1");
  TLO_EXPECT(UBigInt("16384") / "16384" == "1");
  TLO_EXPECT(UBigInt("131072") / "131072" == "1");

  TLO_EXPECT(UBigInt("131072") / "128" == "1024");
  TLO_EXPECT(UBigInt("128") / "131072" == "0");

  TLO_EXPECT(UBigInt("131072") / "1024" == "128");
  TLO_EXPECT(UBigInt("1024") / "131072" == "0");

  TLO_EXPECT(UBigInt("0") / "0" == "0");
  TLO_EXPECT(UBigInt("131072") / "0" == "131072");
  TLO_EXPECT(UBigInt("0") / "131072" == "0");

  TLO_EXPECT(UBigInt("1000000") / "1000000" == "1");
  TLO_EXPECT(UBigInt("1000000") / "999999" == "1");
  TLO_EXPECT(UBigInt("1000100") / "999999" == "1");

  TLO_EXPECT(UBigInt("1") / "1" == "1");
  TLO_EXPECT(UBigInt("131072") / "1" == "131072");
  TLO_EXPECT(UBigInt("1") / "131072" == "0");

  // trailing 0s in dividend
  TLO_EXPECT(UBigInt("42000000") / "42" == "1000000");

  TLO_EXPECT(UBigInt("42999999") / "42" == "1023809");
}

TLO_TEST(testUBigIntModulo) {
  TLO_EXPECT(UBigInt("2") % "2" == "0");
  TLO_EXPECT(UBigInt("16") % "16" == "0");
  TLO_EXPECT(UBigInt("128") % "128" == "0");
  TLO_EXPECT(UBigInt("1024") % "1024" == "0");
  TLO_EXPECT(UBigInt("16384") % "16384" == "0");
  TLO_EXPECT(UBigInt("131072") % "131072" == "0");

  TLO_EXPECT(UBigInt("131072") % "128" == "0");
  TLO_EXPECT(UBigInt("128") % "131072" == "128");

  TLO_EXPECT(UBigInt("131072") % "1024" == "0");
  TLO_EXPECT(UBigInt("1024") % "131072" == "1024");

  TLO_EXPECT(UBigInt("0") % "0" == "0");
  TLO_EXPECT(UBigInt("131072") % "0" == "131072");
  TLO_EXPECT(UBigInt("0") % "131072" == "0");

  TLO_EXPECT(UBigInt("1000000") % "1000000" == "0");
  TLO_EXPECT(UBigInt("1000000") % "999999" == "1");
  TLO_EXPECT(UBigInt("1000100") % "999999" == "101");

  TLO_EXPECT(UBigInt("1") % "1" == "0");
  TLO_EXPECT(UBigInt("131072") % "1" == "0");
  TLO_EXPECT(UBigInt("1") % "131072" == "1");

  TLO_EXPECT(UBigInt("42000000") % "42" == "0");

  TLO_EXPECT(UBigInt("42999999") % "42" == "21");
}

TLO_TEST(testUBigIntFactorial) {
  TLO_EXPECT(UBigInt("0").factorial() == "1");
  TLO_EXPECT(UBigInt("1").factorial() == "1");
  TLO_EXPECT(UBigInt("2").factorial() == "2");
  TLO_EXPECT(UBigInt("3").factorial() == "6");
  TLO_EXPECT(UBigInt("4").factorial() == "24");
  TLO_EXPECT(UBigInt("5").factorial() == "120");
  TLO_EXPECT(UBigInt("6").factorial() == "720");
  TLO_EXPECT(UBigInt("7").factorial() == "5040");
  TLO_EXPECT(UBigInt("8").factorial() == "40320");
  TLO_EXPECT(UBigInt("9").factorial() == "362880");
  TLO_EXPECT(UBigInt("10").factorial() == "3628800");
  TLO_EXPECT(UBigInt("11").factorial() == "39916800");
  TLO_EXPECT(UBigInt("12").factorial() == "479001600");
  TLO_EXPECT(UBigInt("13").factorial() == "6227020800");
  TLO_EXPECT(UBigInt("14").factorial() == "87178291200");
  TLO_EXPECT(UBigInt("15").factorial() == "1307674368000");
  TLO_EXPECT(UBigInt("16").factorial() == "20922789888000");
  TLO_EXPECT(UBigInt("17").factorial() == "355687428096000");
  TLO_EXPECT(UBigInt("18").factorial() == "6402373705728000");
  TLO_EXPECT(UBigInt("19").factorial() == "121645100408832000");
  TLO_EXPECT(UBigInt("20").factorial() == "2432902008176640000");
  TLO_EXPECT(UBigInt("21").factorial() == "51090942171709440000");
  TLO_EXPECT(UBigInt("22").factorial() == "1124000727777607680000");
  TLO_EXPECT(UBigInt("23").factorial() == "25852016738884976640000");
  TLO_EXPECT(UBigInt("24").factorial() == "620448401733239439360000");
  TLO_EXPECT(UBigInt("25").factorial() == "15511210043330985984000000");
  TLO_EXPECT(
      UBigInt("50").factorial() ==
      "30414093201713378043612608166064768844377641568960512000000000000");
  TLO_EXPECT(UBigInt("100").factorial() ==
             "93326215443944152681699238856266700490715968264381621468592963895"
             "21759999322991560894146397615651828625369792082722375825118521091"
             "6864000000000000000000000000");
}

TLO_TEST(testUBigIntOperatorLeftShift) {
  TLO_EXPECT(tlo::toString(UBigInt("")) == "0");
  TLO_EXPECT(tlo::toString(UBigInt("+")) == "0");
  TLO_EXPECT(tlo::toString(UBigInt("-")) == "0");

  TLO_EXPECT(tlo::toString(UBigInt("1")) == "1");
  TLO_EXPECT(tlo::toString(UBigInt("10")) == "10");
  TLO_EXPECT(tlo::toString(UBigInt("100")) == "100");
  TLO_EXPECT(tlo::toString(UBigInt("1000")) == "1000");
  TLO_EXPECT(tlo::toString(UBigInt("10000")) == "10000");
  TLO_EXPECT(tlo::toString(UBigInt("100000")) == "100000");

  TLO_EXPECT(tlo::toString(UBigInt("2")) == "2");
  TLO_EXPECT(tlo::toString(UBigInt("16")) == "16");
  TLO_EXPECT(tlo::toString(UBigInt("128")) == "128");
  TLO_EXPECT(tlo::toString(UBigInt("1024")) == "1024");
  TLO_EXPECT(tlo::toString(UBigInt("16384")) == "16384");
  TLO_EXPECT(tlo::toString(UBigInt("131072")) == "131072");

  TLO_EXPECT(tlo::toString(UBigInt("+1024")) == "1024");
  TLO_EXPECT(tlo::toString(UBigInt("-1024")) == "1024");

  TLO_EXPECT(tlo::toString(UBigInt("0")) == "0");
  TLO_EXPECT(tlo::toString(UBigInt("00")) == "0");
  TLO_EXPECT(tlo::toString(UBigInt("000")) == "0");
  TLO_EXPECT(tlo::toString(UBigInt("0000")) == "0");
  TLO_EXPECT(tlo::toString(UBigInt("0001")) == "1");
}
}  // namespace
