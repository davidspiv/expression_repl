#include "../include/c_historyCache.h"

#include <string>

void HistoryCache::addEntry(const std::string& entry) {
  history.push_back(entry);
}

void HistoryCache::moveForward() {
  if (iter != history.end()) {
    ++iter;
  }
}

void HistoryCache::moveBackward() {
  if (iter != history.begin()) {
    --iter;
  }
}

void HistoryCache::end() { iter = history.end(); }

bool HistoryCache::isLast() { return iter == std::prev(history.end()); }

bool HistoryCache::empty() { return history.empty(); }

std::string HistoryCache::getCurrent() const {
  return history.empty() ? "" : *iter;
}
