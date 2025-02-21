#include "../include/_math.h"

#include <iostream>

#define M_PI 3.14159265358979323846

int _floor(double input) { return static_cast<int>(input); }

double _abs(double value) { return value >= 0 ? value : -value; }

double normalize(double x) { return x - _floor(x / (2 * M_PI)) * M_PI * 2; }

double newtonsMethodRad(int index, double radicand) {
  double guess = radicand / index;
  double delta;

  do {
    double oldGuess = guess;
    guess = (1.0 / index) *
            ((index - 1) * oldGuess + (radicand / _pow(oldGuess, index - 1)));
    delta = guess - oldGuess;
  } while (_abs(delta) > 0.0001);

  return guess;
}

double _pow(double base, double exp) {
  if (exp == 0) return 1;
  if (base == 0) return 0;
  if (base < 0 && exp != _floor(exp)) {
    std::cerr
        << "Error: Negative base with non-integer exponent is undefined in "
           "real numbers."
        << std::endl;
    return -1;
  }

  int integerPart = static_cast<int>(exp);
  double decimalPart = exp - integerPart;
  double result = 1;

  if (_abs(decimalPart) > 0.0001) {
    double root =
        newtonsMethodRad(10, base);  // Approximate 10th root (adjust as needed)
    result *=
        _pow(root, decimalPart * 10);  // Adjust based on fraction approximation
  }

  if (!integerPart) return result;

  if (integerPart < 0) {
    base = 1 / base;
    integerPart = -integerPart;
  }

  double temp = base;
  while (integerPart) {
    if (integerPart % 2) result *= temp;
    temp *= temp;
    integerPart /= 2;
  }

  return result;
}
