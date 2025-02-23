#ifndef INPUT_LINE_H
#define INPUT_LINE_H

#include <string>

#include "historyCache.h"
#include "result.h"

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
  void newExpression();
  void displayResult();
  bool isError();

  void displayInput();
  void handleResult(const std::string &err);
  bool isDisplayable(char ch);
  void handleChar(const char ch);
  void updateExpression(char ch);
};

#endif
