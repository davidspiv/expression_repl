#ifndef HISTORY_CACHE_H
#define HISTORY_CACHE_H

#include <iostream>
#include <list>
#include <string>

class HistoryCache {
 public:
  HistoryCache() : history(), iter(history.end()) {};
  ~HistoryCache() {};
  void addEntry(const std::string& entry);
  void moveForward();
  void moveBackward();
  void end();
  bool isLast();
  std::string getCurrent() const;
  bool empty();

 private:
  std::list<std::string> history;
  std::list<std::string>::iterator iter;
};

#endif
