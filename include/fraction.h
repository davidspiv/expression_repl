#ifndef FRACTION_H
#define FRACTION_H

#include <sstream>

class Fraction {
 private:
  double decVal;
  int num;
  int den;
  int accuracy;
  int gcd(int x, int y);

 public:
  explicit Fraction(const double &input);
  void reduce();
  int getNum() const;
  int getDen() const;
  std::ostringstream getFraction() const;
  ~Fraction() {};
};

Fraction::Fraction(const double &input) {
  accuracy = 1000;
  decVal = input;
  num = decVal * 1000;
  den = 1000;
}

int Fraction::gcd(int x, int y) {
  int remainder = x % y;
  while (remainder) {
    x = y;
    y = remainder;
    remainder = x % y;
  };

  return y;
}

int Fraction::getNum() const { return num; };

int Fraction::getDen() const { return den; };

void Fraction::reduce() {
  int factor = this->gcd(num, den);
  num /= factor;
  den /= factor;
}

std::ostringstream Fraction::getFraction() const {
  std::ostringstream ss;
  ss << std::to_string(num) << " / " << std::to_string(den);
  return ss;
}

#endif
