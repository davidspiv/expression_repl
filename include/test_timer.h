#ifndef TEST_TIMER_H
#define TEST_TIMER_H

#include <chrono>

class Timer {
 public:
  Timer()
      : durationMs(0),
        timePointStart(std::chrono::high_resolution_clock::now()) {};
  ~Timer();

 private:
  double durationMs;
  std::chrono::time_point<std::chrono::high_resolution_clock> timePointStart;
  std::chrono::time_point<std::chrono::high_resolution_clock> timePointEnd;
};

#endif
