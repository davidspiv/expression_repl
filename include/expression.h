#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "historyCache.h"
#include "result.h"

class Expression {
 public:
  Expression() { this->reset(); };
  ~Expression() {};

  enum InputState { INPUT, HISTORY };

  size_t getCursorIndex() const;
  std::string getInput() const;
  InputState getInputState() const;
  void handleChar(const char ch, HistoryCache &historyCache);
  void displayFinalResult();
  bool isError() const;

 private:
  InputState inputState;

  size_t cursorIndex;
  std::string input;
  std::string savedInput;
  std::string result;
  std::string errMessage;
  bool isSecondLine;

 private:
  void setInput(const std::string &text);
  void backspace();
  void reset();
  void updateDisplay(const std::string &err);
};

#endif
