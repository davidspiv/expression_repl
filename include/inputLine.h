#ifndef INPUT_LINE_H
#define INPUT_LINE_H

#include <string>

#include "historyCache.h"

class InputLine {
 private:
  size_t cursorIndex;
  std::string text;

  std::string result;
  std::string errMessage;
  bool isSecondLine;

 public:
  InputLine()
      : cursorIndex(0),
        text(""),
        result(""),
        errMessage(""),
        isSecondLine(false) {};
  ~InputLine() {};

  static HistoryCache historyCache;

  size_t getCursorIndex() const;
  std::string getText() const;
  void setText(const std::string &text);
  void erase();
  void insert(char ch);
  void reset();
  void operator++();
  void operator--();
  void newExpression();
  void displayResult();
  bool isError();
};

#endif
