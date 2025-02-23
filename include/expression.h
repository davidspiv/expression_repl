#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>

#include "historyCache.h"
#include "result.h"

class Expression {
 private:
  size_t cursorIndex;
  std::string text;

  std::string result;
  std::string errMessage;
  bool isSecondLine;

 public:
  Expression()
      : cursorIndex(0),
        text(""),
        result(""),
        errMessage(""),
        isSecondLine(false) {};
  ~Expression() {};

  static HistoryCache historyCache;

  size_t getCursorIndex() const;
  std::string getText() const;
  void setText(const std::string &text);
  void backspace();
  void insert(char ch);
  void reset();

  void createExpression();
  void displayFinalResult();
  bool isError();
  void displayInput(const std::string &err);
  void handleChar(const char ch);
  void updateExpression(char ch);
};

#endif
