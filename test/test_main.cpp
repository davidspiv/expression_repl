#include <iostream>

#include "../include/test_taylorSeries.h"
#include "../include/test_timer.h"

int main() {
  {
    Timer timer;
    testTaylorSeries();
  }
}
