#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#endif

#include <iostream>
#include <string>

#include "../include/evalRpn.h"
#include "../include/historyCache.h"
#include "../include/inputLine.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/parser.h"

static HistoryCache historyCache;

bool isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) || opRank.count(std::string(1, ch)) ||
         ch == ')' || ch == ' ';
}

void handleResult(const InputLine &inputLine, ResultAsString &result,
                  const std::string &errMessage = "") {
  result.errMessage = errMessage;
  displayInput(inputLine, result);
};

void handleChar(const char ch, InputLine &inputLine) {
#ifdef _WIN32
  if (ch == 224) {  // handle ANSI escape sequence

    char escCode = _getch();

    switch (escCode) {
      case 72:  // up arrow
        std::cout << "hello!";
        historyCache.moveBackward();
        inputLine.setText(historyCache.getCurrent());
        break;

      case 80:  // down arrow
        historyCache.moveForward();
        inputLine.setText(historyCache.getCurrent());
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
          historyCache.moveBackward();
          inputLine.setText(historyCache.getCurrent());
          break;

        case 'B':  // down arrow
          historyCache.moveForward();
          inputLine.setText(historyCache.getCurrent());
          break;

        case 'D':
          if (inputLine.getCursorIndex()) {
            std::cout << CURSOR_LEFT << std::flush;
            --inputLine;
          }
          break;

        case 'C':
          if (inputLine.getCursorIndex() < inputLine.getText().length()) {
            std::cout << CURSOR_RIGHT << std::flush;
            ++inputLine;
          }
          break;
      }
    }

  } else if (ch == 0x7F && inputLine.getCursorIndex()) {  // handle backspace
    std::cout << "\b \b";
    inputLine.erase();

  } else if (isDisplayable(ch)) {  // handle character to display
    inputLine.insert(ch);
  }
#endif
}

ResultAsString updateExpression(char ch, InputLine &inputLine) {
  ResultAsString result;

  handleChar(ch, inputLine);

  if (inputLine.getText().empty()) {
    handleResult(inputLine, result, "empty input");
    return result;
  }

  ResultAsTokens algResult = lexer(inputLine.getText());

  if (!algResult.errMessage.empty()) {
    handleResult(inputLine, result, algResult.errMessage);
    return result;
  }

  Token::Type lastTokenType = algResult.tokens.back().getType();

  if (lastTokenType == Token::UnaryOp || lastTokenType == Token::BinaryOp) {
    handleResult(inputLine, result, "hanging operator");
    return result;
  }

  ResultAsTokens rpnResult = parser(algResult.tokens);

  if (!rpnResult.errMessage.empty()) {
    handleResult(inputLine, result, rpnResult.errMessage);
    return result;
  }

  result = evalRpn(rpnResult.tokens);

  //   inputLine.setCursorIndex(0);
  handleResult(inputLine, result);
  return result;
}

ResultAsString newExpression(InputLine &inputLine) {
  char ch;
  ResultAsString result;

  inputLine.reset();

  // TEST HISTORY
  if (historyCache.empty()) {
    historyCache.addEntry("1*1");
    historyCache.addEntry("2*2");
    historyCache.addEntry("3*3");
  }

  while (readNextChar(ch) && ch != '\n') {
    result = updateExpression(ch, inputLine);
  }

  if (historyCache.isEnd() ||
      historyCache.getCurrent() != inputLine.getText()) {
    historyCache.addEntry(inputLine.getText());
  }

  if (!result.errMessage.empty()) {
    displayError(result, inputLine.getCursorIndex());
    return newExpression(inputLine);
  }

  historyCache.end();
  return result;
};
