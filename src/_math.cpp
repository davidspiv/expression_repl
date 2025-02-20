#include <iostream>

#define M_PI 3.14159265358979323846

int _floor(double input) { return static_cast<int>(input); }

double _abs(double value) { return value >= 0 ? value : -value; }

double normalize(double x) { return x - _floor(x / M_PI) * M_PI; }

double _pow(double base, int exp) {
  if (!exp || base == 1) {
    return 1;
  } else if (!(exp % 2)) {
    const double y = _pow(base, exp / 2);
    return y * y;
  } else {
    return base * _pow(base, exp - 1);
  }
}
