#include "../include/taylorSeries.h"

#include <math.h>
#include <time.h>

#include <cassert>
#include <iostream>
#include <random>

#include "../include/token.h"

using namespace std;

// static double maxDouble = std::numeric_limits<double>::max();
// static double minDouble = std::numeric_limits<double>::min();

bool isEqual(double a, double b, double accuracy = 1e-6) {
  return abs(a - b) < accuracy;
};

double getRandomDouble(double lower = -10000, double upper = 10000) {
  static std::mt19937 generator(
      static_cast<unsigned>(time(nullptr)));  // Seed with current time
  std::uniform_real_distribution<double> distribution(lower, upper);
  return distribution(generator);
}

int main() {
  Token token;
  double rand = getRandomDouble();
  cout << "RANDOM DOUBLE: " << rand << endl;

  token.setType(Token::UnaryOp);

  cout << "TEST" << (taylorSeries(token, 2990.35)) << endl;
  cout << "ANSWER" << sin(2990.35) << endl;
  token.setSymbol("sin");
  assert(isEqual(taylorSeries(token, 2990.35), sin(2990.35)));

  token.setSymbol("cos");
  assert(isEqual(taylorSeries(token, 2990.35), cos(2990.35)));
}
