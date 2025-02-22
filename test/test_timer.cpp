#include "../include/test_timer.h"

#include <chrono>
#include <iostream>

#include "../include/taylorSeries.h"

Timer::~Timer() {
  timePointEnd = std::chrono::high_resolution_clock::now();
  auto start =
      std::chrono::time_point_cast<std::chrono::microseconds>(timePointStart)
          .time_since_epoch()
          .count();
  auto end =
      std::chrono::time_point_cast<std::chrono::microseconds>(timePointEnd)
          .time_since_epoch()
          .count();

  auto duration = end - start;
  durationMs = duration * 00.1;
  std::cout << durationMs << " ms" << std::endl;
}
