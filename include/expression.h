#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "historyCache.h"
#include "result.h"

class Expression {
 public:
  Expression() { this->reset(); };
  ~Expression() {};

  size_t getCursorIndex() const;
  std::string getInput() const;
  void createExpression();  // controller
  void displayFinalResult();
  bool isError() const;

 private:
  static HistoryCache historyCache;
  enum InputState { INPUT, HISTORY };
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
  void update(char ch);
  void reset();
  void displayInput(const std::string &err);
  void handleChar(const char ch);
};

#endif
