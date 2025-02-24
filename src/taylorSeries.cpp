#include <iostream>
#include <string>

#include "../include/_math.h"
#include "../include/c_token.h"

void createFactorialCache(double *factorialCache, size_t iterations) {
  factorialCache[0] = 1;
  for (size_t i = 1; i <= iterations; i++) {
    factorialCache[i] = i * factorialCache[i - 1];
  }
}

double createCoeff(double denominator, const bool &isPositive) {
  double coeff = (1 / denominator);
  coeff = isPositive ? coeff : -coeff;
  return coeff;
}

double taylorSeries(const Token &token, double radians) {
  const double normRadians = normalize(radians);
  const size_t terms = 50;
  double factorialCache[terms + 1];
  bool isPositive = terms % 4 && terms % 4 != 3;
  double evenResult = 0;
  double oddResult = 0;

  createFactorialCache(&factorialCache[0], terms);

  // Horner's method
  for (int i = terms; i > 0; i--) {
    if (i % 2) {
      evenResult += createCoeff(factorialCache[i], isPositive);
      isPositive = !isPositive;
    } else {
      oddResult += createCoeff(factorialCache[i], isPositive);
    }
    evenResult *= normRadians;
    oddResult *= normRadians;
  }

  oddResult = 1 - oddResult;

  if (token.getSymbol() == "sin") {
    return evenResult;

  } else if (token.getSymbol() == "cos") {
    return oddResult;

  } else if (token.getSymbol() == "tan") {
    return evenResult / oddResult;

  } else if (token.getSymbol() == "csc") {
    return 1 / evenResult;

  } else if (token.getSymbol() == "sec") {
    return 1 / oddResult;

  } else if (token.getSymbol() == "cot") {
    return oddResult / evenResult;
  }
  return 0;
}
