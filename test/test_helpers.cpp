#include "../include/test_helpers.h"

#include <limits>
#include <random>

double getRandomDouble(double min, double max) {
  static std::mt19937 generator(
      static_cast<unsigned>(time(nullptr)));  // Seed with current time
  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(generator);
}

bool equiv(double a, double b) {
  double accuracy = 1.0E-6;
  return std::abs(a - b) < accuracy;
};
