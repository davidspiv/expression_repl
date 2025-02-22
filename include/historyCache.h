#ifndef HISTORY_CACHE_H
#define HISTORY_CACHE_H

#include <iostream>
#include <list>
#include <string>

class HistoryCache {
 public:
  void addEntry(const std::string& entry);

  void moveForward();

  void moveBackward();

  void end();

  bool isEnd();

  bool isBeginning();

  std::string getCurrent() const;

  bool empty();

 private:
  std::list<std::string> history;
  std::list<std::string>::iterator iter = history.end();
};

#endif
