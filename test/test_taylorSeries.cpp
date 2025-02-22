
#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include "../include/taylorSeries.h"
#include "../include/test_helpers.h"
#include "../include/token.h"

using namespace std;

double lower = std::numeric_limits<double>::min();
double upper = std::numeric_limits<double>::max();
double testVal = 328293847234;
size_t failed = 0;
ostringstream out;

void testTaylorSeries() {
  out << "Taylor Series Test:\n";

  Token token;
  token.setType(Token::UnaryOp);
  token.setSymbol("sin");

  double test = taylorSeries(token, testVal);
  double answer = sin(testVal);

  if (!Equiv(test, answer)) {
    out << "INPUT: sin(" << testVal << ") ,TEST: " << test
        << ", ANSWER: " << answer << "\n";
    failed++;
  }

  token.setSymbol("cos");
  test = taylorSeries(token, testVal);
  answer = cos(testVal);

  if (!Equiv(test, answer)) {
    out << "INPUT: cos(" << testVal << ") ,TEST: " << test
        << ", ANSWER: " << answer << "\n";
    failed++;
  }

  token.setSymbol("tan");
  test = taylorSeries(token, testVal);
  answer = tan(testVal);

  if (!Equiv(test, answer)) {
    out << "INPUT: tan(" << testVal << ") ,TEST: " << test
        << ", ANSWER: " << answer << "\n";
    failed++;
  }

  token.setSymbol("csc");
  test = taylorSeries(token, testVal);
  answer = 1 / sin(testVal);

  if (!Equiv(test, answer)) {
    out << "INPUT: csc(" << testVal << ") ,TEST: " << test
        << ", ANSWER: " << answer << "\n";
    failed++;
  }

  token.setSymbol("sec");
  test = taylorSeries(token, testVal);
  answer = 1 / cos(testVal);

  if (!Equiv(test, answer)) {
    out << "INPUT: sec(" << testVal << ") ,TEST: " << test
        << ", ANSWER: " << answer << "\n";
    failed++;
  }

  token.setSymbol("cot");
  test = taylorSeries(token, testVal);
  answer = 1 / tan(testVal);

  if (!Equiv(test, answer)) {
    out << "INPUT: cot(" << testVal << ") ,TEST: " << test
        << ", ANSWER: " << answer << "\n";
    failed++;
  }

  if (!failed) {
    out << "All tests passed.\n";
  } else {
    out << failed << " test(s) failed.\n";
  }

  cout << out.str() << flush;
}
