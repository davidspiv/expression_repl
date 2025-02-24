#include "../include/expression.h"

#include <string>

#include "../include/historyCache.h"
#include "../include/io.h"

size_t Expression::getCursorIndex() const { return cursorIndex; }

std::string Expression::getInput() const { return input; }

Expression::InputState Expression::getInputState() const { return inputState; }

std::string Expression::getResult() const { return result; }

std::string Expression::getErrMessage() const { return errMessage; }

void Expression::setInput(const std::string &text) {
  input = text;
  cursorIndex = input.length();
}

void Expression::setResult(const std::string &result) {
  this->result = result;
};

void Expression::setError(const std::string &errMessage) {
  this->errMessage = errMessage;
};

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
bool Expression::handleChar(const char ch, HistoryCache &historyCache) {
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

          if (inputState == INPUT) return false;

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
  return true;
}
