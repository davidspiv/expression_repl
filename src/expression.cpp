#include "../include/expression.h"

#include <sstream>
#include <string>

#include "../include/evalRpn.h"
#include "../include/historyCache.h"
#include "../include/ioHelpers.h"
#include "../include/lexer.h"
#include "../include/parser.h"

HistoryCache Expression::historyCache;

size_t Expression::getCursorIndex() const { return cursorIndex; }

std::string Expression::getText() const { return text; }

void Expression::setText(const std::string &text) {
  this->text = text;
  cursorIndex = text.length();
}

void Expression::backspace() {
  if (cursorIndex > text.length()) return;
  --cursorIndex;
  text.erase(text.begin() + cursorIndex);
};

void Expression::insert(char ch) {
  text.insert(cursorIndex, std::string(1, ch));
  ++cursorIndex;
};

void Expression::reset() {
  cursorIndex = 0;
  text = "";
  result = "";
  errMessage = "";
  isSecondLine = false;
}

bool Expression::isError() { return !errMessage.empty(); };

void Expression::createExpression() {
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

void Expression::handleChar(const char ch) {
#ifdef _WIN32
  if (ch == 224) {  // handle ANSI escape sequence

    char escCode = _getch();

    switch (escCode) {
      case 72:  // up arrow
        std::cout << "hello!";
        Expression::historyCache.moveBackward();
        text = Expression::historyCache.getCurrent();
        break;

      case 80:  // down arrow
        Expression::historyCache.moveForward();
        text = Expression::historyCache.getCurrent();
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
          Expression::historyCache.moveBackward();
          text = Expression::historyCache.getCurrent();
          break;

        case 'B':  // down arrow
          Expression::historyCache.moveForward();
          text = Expression::historyCache.getCurrent();
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
    backspace();

  } else if (isDisplayable(ch)) {  // handle character to display
    insert(ch);
  }
#endif
}

void Expression::updateExpression(char ch) {
  handleChar(ch);

  if (text.empty()) {
    displayInput("empty input");
    return;
  }

  ResultAsTokens algResult = lexer(text);

  if (!algResult.errMessage.empty()) {
    displayInput(algResult.errMessage);
    return;
  }

  Token::Type lastTokenType = algResult.tokens.back().getType();

  if (lastTokenType == Token::UnaryOp || lastTokenType == Token::BinaryOp) {
    displayInput("hanging operator");
    return;
  }

  ResultAsTokens rpnResult = parser(algResult.tokens);

  if (!rpnResult.errMessage.empty()) {
    displayInput(rpnResult.errMessage);
    return;
  }

  ResultAsString resultAsString = evalRpn(rpnResult.tokens);
  result = resultAsString.str;

  displayInput(resultAsString.errMessage);
}

void Expression::displayInput(const std::string &err) {
  std::ostringstream out;

  errMessage = err;

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

void Expression::displayFinalResult() {
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
