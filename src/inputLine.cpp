#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#endif

#include <iostream>
#include <sstream>
#include <string>

#include "../include/evalRpn.h"
#include "../include/historyCache.h"
#include "../include/inputLine.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/parser.h"

HistoryCache InputLine::historyCache;

size_t InputLine::getCursorIndex() const { return cursorIndex; }

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

  // TEST HISTORY
  if (this->historyCache.empty()) {
    historyCache.addEntry("1*1");
    historyCache.addEntry("2*2");
    historyCache.addEntry("3*3");
  }

  while (readNextChar(ch) && ch != '\n') {
    updateExpression(ch);
  }

  if (historyCache.isEnd() || historyCache.getCurrent() != text) {
    historyCache.addEntry(text);
  }

  historyCache.end();
};

bool InputLine::isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) || opRank.count(std::string(1, ch)) ||
         ch == ')' || ch == ' ';
}

void InputLine::displayInput() {
  std::ostringstream out;

  if (!result.empty() && errMessage.empty()) {
    out << '\n' << CLEAR << GREY << stod(result) << WHITE << PREV_LINE;

    isSecondLine = true;

  } else if (isSecondLine) {
    out << '\n' << CLEAR << PREV_LINE;
  }

  out << '\r' << CLEAR << ">  " << text;

  for (size_t i = 0; i < text.length() - cursorIndex; i++) {
    out << CURSOR_LEFT;
  }

  std::cout << out.str() << std::flush;
}

void InputLine::handleResult(const std::string &err) {
  errMessage = err;
  displayInput();
};

void InputLine::handleChar(const char ch) {
#ifdef _WIN32
  if (ch == 224) {  // handle ANSI escape sequence

    char escCode = _getch();

    switch (escCode) {
      case 72:  // up arrow
        std::cout << "hello!";
        InputLine::historyCache.moveBackward();
        text = InputLine::historyCache.getCurrent();
        break;

      case 80:  // down arrow
        InputLine::historyCache.moveForward();
        text = InputLine::historyCache.getCurrent();
        break;
    }

  } else if (isDisplayable(ch)) {  // handle character to display
    inputLine.insert(ch);
  }
#else
  if (ch == '\033') {  // handle ANSI escape sequence

    char escCode[2];

    if (readNextChar(escCode[0]) && readNextChar(escCode[1]) &&
        escCode[0] == '[') {
      switch (escCode[1]) {
        case 'A':  // up arrow
          InputLine::historyCache.moveBackward();
          text = InputLine::historyCache.getCurrent();
          break;

        case 'B':  // down arrow
          InputLine::historyCache.moveForward();
          text = InputLine::historyCache.getCurrent();
          break;

        case 'D':
          if (cursorIndex) {
            std::cout << CURSOR_LEFT << std::flush;
            --cursorIndex;
          }
          break;

        case 'C':
          if (cursorIndex < text.length()) {
            std::cout << CURSOR_RIGHT << std::flush;
            ++cursorIndex;
          }
          break;
      }
    }

  } else if (ch == 0x7F && cursorIndex) {  // handle backspace
    std::cout << "\b \b";
    this->erase();

  } else if (isDisplayable(ch)) {  // handle character to display
    this->insert(ch);
  }
#endif
}

void InputLine::updateExpression(char ch) {
  handleChar(ch);

  if (text.empty()) {
    handleResult("empty input");
    return;
  }

  ResultAsTokens algResult = lexer(text);

  if (!algResult.errMessage.empty()) {
    handleResult(algResult.errMessage);
    return;
  }

  Token::Type lastTokenType = algResult.tokens.back().getType();

  if (lastTokenType == Token::UnaryOp || lastTokenType == Token::BinaryOp) {
    handleResult("hanging operator");
    return;
  }

  ResultAsTokens rpnResult = parser(algResult.tokens);

  if (!rpnResult.errMessage.empty()) {
    handleResult(rpnResult.errMessage);
    return;
  }

  ResultAsString resultAsString = evalRpn(rpnResult.tokens);
  result = resultAsString.str;

  handleResult(resultAsString.errMessage);
}
