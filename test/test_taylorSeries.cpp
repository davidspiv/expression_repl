
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

#include "../include/taylorSeries.h"
#include "../include/test_helpers.h"
#include "../include/token.h"

using namespace std;

void testTaylorSeries() {
  double testVal = 234234;
  size_t failed = 0;
  ostringstream out;

  out << "\ntaylorSeries test:\n";

  Token token;
  token.setType(Token::UnaryOp);
  token.setSymbol("sin");

  double test = taylorSeries(token, testVal);
  double answer = sin(testVal);

  if (!equiv(test, answer)) {
    out << "INPUT: sin(" << testVal << "), RSLT: " << test
        << ", ANSW: " << answer << "\n";
    failed++;
  }

  token.setSymbol("cos");
  test = taylorSeries(token, testVal);
  answer = cos(testVal);

  if (!equiv(test, answer)) {
    out << "INPUT: cos(" << testVal << "), RSLT: " << test
        << ", ANSW: " << answer << "\n";
    failed++;
  }

  token.setSymbol("tan");
  test = taylorSeries(token, testVal);
  answer = tan(testVal);

  if (!equiv(test, answer)) {
    out << "INPUT: tan(" << testVal << "), RSLT: " << test
        << ", ANSW: " << answer << "\n";
    failed++;
  }

  token.setSymbol("csc");
  test = taylorSeries(token, testVal);
  answer = 1 / sin(testVal);

  if (!equiv(test, answer)) {
    out << "INPUT: csc(" << testVal << "), RSLT: " << test
        << ", ANSW: " << answer << "\n";
    failed++;
  }

  token.setSymbol("sec");
  test = taylorSeries(token, testVal);
  answer = 1 / cos(testVal);

  if (!equiv(test, answer)) {
    out << "INPUT: sec(" << testVal << "), RSLT: " << test
        << ", ANSW: " << answer << "\n";
    failed++;
  }

  token.setSymbol("cot");
  test = taylorSeries(token, testVal);
  answer = 1 / tan(testVal);

  if (!equiv(test, answer)) {
    out << "INPUT: cot(" << testVal << "), RSLT: " << test
        << ", ANSW: " << answer << "\n";
    failed++;
  }

  if (!failed) {
    out << "All tests passed.\n";
  } else {
    out << failed << " test(s) failed.\n";
  }

  cout << out.str() << flush;
}
