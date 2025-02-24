#include "../include/expression.h"

#include <sstream>
#include <string>

#include "../include/evalRpn.h"
#include "../include/historyCache.h"
#include "../include/ioHelpers.h"
#include "../include/lexer.h"
#include "../include/parser.h"

size_t Expression::getCursorIndex() const { return cursorIndex; }

std::string Expression::getInput() const { return input; }

Expression::InputState Expression::getInputState() const { return inputState; }

void Expression::setInput(const std::string &text) {
  input = text;
  cursorIndex = input.length();
}

void Expression::reset() {
  inputState = INPUT;
  cursorIndex = 0;
  input = "";
  savedInput = "";
  result = "";
  errMessage = "";
  isSecondLine = false;
}

// removes a character from input line at cursor index
void Expression::backspace() {
  if (cursorIndex > input.length()) return;
  --cursorIndex;
  input.erase(input.begin() + cursorIndex);
};

// Checks for any errors that occurred during the evaluation process
bool Expression::isError() const { return !errMessage.empty(); };

// evaluates expression with additional char
void Expression::handleChar(const char ch, HistoryCache &historyCache) {
#ifdef _WIN32
  if (ch == 224) {  // handle ANSI escape sequence

    char escCode = _getch();

    switch (escCode) {
      case 72:  // up arrow
        std::cout << "hello!";
        Expression::historyCache.moveBackward();
        input = Expression::historyCache.getCurrent();
        break;

      case 80:  // down arrow
        Expression::historyCache.moveForward();
        input = Expression::historyCache.getCurrent();
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
          if (inputState == INPUT) {
            inputState = HISTORY;
            savedInput = input;
          }
          historyCache.moveBackward();
          this->setInput(historyCache.getCurrent());
          break;

        case 'B':  // down arrow

          if (inputState == INPUT) return;

          if (historyCache.isLast()) {
            inputState = INPUT;
            this->setInput(savedInput);
            historyCache.moveForward();  // reset iter
          } else {
            historyCache.moveForward();
            this->setInput(historyCache.getCurrent());
          }

          break;

        case 'D':
          if (cursorIndex) {
            std::cout << CURSOR_LEFT << std::flush;
            --cursorIndex;
          }
          break;

        case 'C':
          if (cursorIndex < input.length()) {
            std::cout << CURSOR_RIGHT << std::flush;
            ++cursorIndex;
          }
          break;
      }
    }
  } else if (ch == 0x7F && cursorIndex) {  // handle backspace
    std::cout << "\b \b";
    this->backspace();

  } else if (isDisplayable(ch)) {  // handle character to display
    input.insert(cursorIndex, std::string(1, ch));
    ++cursorIndex;
  }
#endif

  if (input.empty()) {
    updateDisplay("empty input");
    return;
  }

  ResultAsTokens algResult = lexer(input);

  if (!algResult.errMessage.empty()) {
    updateDisplay(algResult.errMessage);
    return;
  }

  Token::Type lastTokenType = algResult.tokens.back().getType();

  if (lastTokenType == Token::UnaryOp || lastTokenType == Token::BinaryOp) {
    updateDisplay("hanging operator");
    return;
  }

  ResultAsTokens rpnResult = parser(algResult.tokens);

  if (!rpnResult.errMessage.empty()) {
    updateDisplay(rpnResult.errMessage);
    return;
  }

  ResultAsString resultAsString = evalRpn(rpnResult.tokens);
  result = resultAsString.str;

  updateDisplay(resultAsString.errMessage);
}

void Expression::updateDisplay(const std::string &err) {
  std::ostringstream out;

  errMessage = err;

  if (!result.empty() && errMessage.empty()) {
    out << '\n' << CLEAR << GREY << stod(result) << WHITE << PREV_LINE;

    isSecondLine = true;
  } else if (isSecondLine) {
    out << '\n' << CLEAR << PREV_LINE;
  }

  out << '\r' << CLEAR << ">  " << input;

  for (size_t i = 0; i < input.length() - cursorIndex; i++) {
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
