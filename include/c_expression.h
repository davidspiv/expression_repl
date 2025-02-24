#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "c_historyCache.h"
#include "result.h"

class Expression {
 public:
  Expression() { this->reset(); };
  ~Expression() {};

  enum InputState { INPUT, HISTORY };

  size_t getCursorIndex() const;
  std::string getInput() const;
  InputState getInputState() const;
  std::string getResult() const;
  std::string getErrMessage() const;
  bool handleChar(const char ch, HistoryCache &historyCache);
  void setResult(const std::string &result);
  void setError(const std::string &errMessage);
  bool isError() const;
  void reset();

  bool isSecondLine;

 private:
  InputState inputState;

  size_t cursorIndex;
  std::string input;
  std::string savedInput;
  std::string result;
  std::string errMessage;

 private:
  void setInput(const std::string &text);
  void backspace();
};

#endif
