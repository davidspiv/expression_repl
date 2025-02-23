#include "../include/inputLine.h"

#include <iostream>
#include <sstream>
#include <string>

#include "../include/expression.h"
#include "../include/historyCache.h"
#include "../include/io.h"

size_t InputLine::getCursorIndex() const { return cursorIndex; }

HistoryCache InputLine::historyCache;

std::string InputLine::getText() const { return text; }

void InputLine::setText(const std::string &text) {
  this->text = text;
  cursorIndex = text.length();
}

void InputLine::erase() {
  if (cursorIndex > text.length()) return;
  --cursorIndex;
  text.erase(text.begin() + cursorIndex);
};

void InputLine::insert(char ch) {
  text.insert(cursorIndex, std::string(1, ch));
  ++cursorIndex;
};

void InputLine::reset() {
  cursorIndex = 0;
  text = "";
  result = "";
  errMessage = "";
  isSecondLine = false;
}

void InputLine::operator++() {
  ++cursorIndex;
  return;
};

void InputLine::operator--() {
  --cursorIndex;
  return;
};

void InputLine::displayResult() {
  std::ostringstream out;

  if (errMessage.size()) {
    out << '\n' << CLEAR << GREY << errMessage << WHITE << PREV_LINE;

    for (size_t i = 0; i < cursorIndex + 3; i++) {
      out << CURSOR_RIGHT;
    }

  } else if (!result.empty()) {
    out << '\n' << YELLOW << CLEAR << stod(result) << WHITE << '\n' << ">  ";
    this->reset();
  }

  std::cout << out.str() << std::flush;
}

bool InputLine::isError() { return !errMessage.empty(); };

void InputLine::newExpression() {
  char ch;
  ResultAsString resultAsString;

  // TEST HISTORY
  if (this->historyCache.empty()) {
    historyCache.addEntry("1*1");
    historyCache.addEntry("2*2");
    historyCache.addEntry("3*3");
  }

  while (readNextChar(ch) && ch != '\n') {
    resultAsString = updateExpression(ch, *this);
  }

  result = resultAsString.str;
  errMessage = resultAsString.errMessage;

  if (historyCache.isEnd() || historyCache.getCurrent() != text) {
    historyCache.addEntry(text);
  }

  historyCache.end();
};
