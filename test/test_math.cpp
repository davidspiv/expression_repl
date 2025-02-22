#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>

#include "../include/_math.h"
#include "../include/test_helpers.h"

using namespace std;

static double maxInt = std::numeric_limits<int>::max();
// static double minInt = std::numeric_limits<int>::min();

void testFloor() {
  size_t failed = 0;
  ostringstream out;
  double num = maxInt - .94675983;
  double test = _floor(num);
  double answ = floor(num);

  out << "\n_floor test:\n";

  if (!equiv(test, answ)) {
    out << "INPUT: " << num << ", TEST: " << test << ", ANSW: " << answ << "\n";
    failed++;
  }

  if (!failed) {
    out << "All tests passed.\n";
  } else {
    out << failed << " test(s) failed.\n";
  }

  cout << out.str() << flush;
}

void testNormalize() {
  size_t failed = 0;
  ostringstream out;
  double num = maxInt;
  double test = normalize(num);
  bool testResult =
      test >= 0 && test < (2 * M_PI) && equiv(sin(num), sin(test));

  out << "\nnormalize test:\n";

  if (!testResult) {
    out << "INPUT: " << num << ", ANSW: " << test << "\n";
    failed++;
  }

  if (!failed) {
    out << "All tests passed.\n";
  } else {
    out << failed << " test(s) failed.\n";
  }

  cout << out.str() << flush;
}
