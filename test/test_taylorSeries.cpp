#include <time.h>

#include <cassert>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>

#include "../include/taylorSeries.h"
#include "../include/token.h"

using namespace std;

// static double maxDouble = std::numeric_limits<double>::max();
// static double minDouble = std::numeric_limits<double>::min();

bool Equiv(double a, double b, double accuracy = 1e-6) {
  return abs(a - b) < accuracy;
};

double lower = std::numeric_limits<double>::min();
double upper = std::numeric_limits<double>::max();

double getRandomDouble(double min = lower, double max = upper) {
  static std::mt19937 generator(
      static_cast<unsigned>(time(nullptr)));  // Seed with current time
  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(generator);
}

void testTaylorSeries() {
  cout << "Taylor Series Test" << endl;

  Token token;
  token.setType(Token::UnaryOp);
  token.setSymbol("sin");

  double rand = getRandomDouble();
  double test = taylorSeries(token, rand);
  double answer = sin(rand);

  if (!Equiv(test, answer)) {
    cout << "INPUT: sin(" << rand << ") ,TEST: " << test
         << ", ANSWER: " << answer << endl;
  }

  token.setSymbol("cos");
  test = taylorSeries(token, rand);
  answer = cos(rand);

  if (!Equiv(test, answer)) {
    cout << "INPUT: cos(" << rand << ") ,TEST: " << test
         << ", ANSWER: " << answer << endl;
  }

  token.setSymbol("tan");
  test = taylorSeries(token, rand);
  answer = tan(rand);

  if (!Equiv(test, answer)) {
    cout << "INPUT: tan(" << rand << ") ,TEST: " << test
         << ", ANSWER: " << answer << endl;
  }

  token.setSymbol("csc");
  test = taylorSeries(token, rand);
  answer = 1 / sin(rand);

  if (!Equiv(test, answer)) {
    cout << "INPUT: csc(" << rand << ") ,TEST: " << test
         << ", ANSWER: " << answer << endl;
  }

  token.setSymbol("sec");
  test = taylorSeries(token, rand);
  answer = 1 / cos(rand);

  if (!Equiv(test, answer)) {
    cout << "INPUT: sec(" << rand << ") ,TEST: " << test
         << ", ANSWER: " << answer << endl;
  }

  token.setSymbol("cot");
  test = taylorSeries(token, rand);
  answer = 1 / tan(rand);

  if (!Equiv(test, answer)) {
    cout << "INPUT: cot(" << rand << ") ,TEST: " << test
         << ", ANSWER: " << answer << endl;
  }
}
