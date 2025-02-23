#include <deque>
#include <iostream>
#include <string>

#include "../include/evalRpn.h"
#include "../include/historyCache.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/result.h"
#include "../include/token.h"

using namespace std;

static HistoryCache historyCache;
static size_t cursorIndex = 0;
static string input;

bool isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) || opRank.count(string(1, ch)) ||
         ch == ')' || ch == ' ';
}

void handleResult(StringResult &result, const string &errMessage = "") {
  result.errMessage = errMessage;
  displayInput(input, result, cursorIndex);
};

StringResult updateExpression(char ch) {
  if (ch == '\033') {  // handle ANSI escape sequence

    char escCode[2];

    if (readNextChar(escCode[0]) && readNextChar(escCode[1]) &&
        escCode[0] == '[') {
      switch (escCode[1]) {
        case 'A':  // up arrow
          historyCache.moveBackward();
          input = historyCache.getCurrent();
          cursorIndex = input.length();
          break;

        case 'B':  // down arrow
        {
          historyCache.moveForward();
          input = historyCache.getCurrent();
          cursorIndex = input.length();
        } break;

        case 'D':
          if (cursorIndex) {
            cout << CURSOR_LEFT << flush;
            cursorIndex--;
          }
          break;

        case 'C':
          if (cursorIndex < input.length()) {
            cout << CURSOR_RIGHT << flush;
            cursorIndex++;
          }
          break;
      }
    }

  } else if (ch == 0x7F && cursorIndex) {  // handle backspace
    cout << "\b \b";
    cursorIndex--;
    input.erase(input.begin() + cursorIndex);

  } else if (isDisplayable(ch)) {  // handle character to display
    input.insert(cursorIndex, string(1, ch));
    cursorIndex++;
  }

  StringResult result;

  if (input.empty()) {
    handleResult(result, "empty input");
    return result;
  }

  TokensResult algResult = lexer(input);

  if (!algResult.errMessage.empty()) {
    handleResult(result, algResult.errMessage);
    return result;
  }

  Token::Type lastTokenType = algResult.tokens.back().getType();

  if (lastTokenType == Token::UnaryOp || lastTokenType == Token::BinaryOp) {
    handleResult(result, "hanging operator");
    return result;
  }

  TokensResult rpnResult = parser(algResult.tokens);

  if (!rpnResult.errMessage.empty()) {
    handleResult(result, rpnResult.errMessage);
    return result;
  }

  result = evalRpn(rpnResult.tokens);

  handleResult(result);
  return result;
}

StringResult newExpression() {
  char ch;
  StringResult result;

  if (!cursorIndex) {
    input = "";
  }

  // TEST HISTORY
  if (historyCache.empty()) {
    historyCache.addEntry("1*1");
    historyCache.addEntry("2*2");
    historyCache.addEntry("3*3");
  }

  while (readNextChar(ch) && ch != '\n') {
    result = updateExpression(ch);
  }

  if (historyCache.isEnd() || historyCache.getCurrent() != input) {
    historyCache.addEntry(input);
  }

  if (!result.errMessage.empty()) {
    displayError(result, cursorIndex);
    return newExpression();
  }

  historyCache.end();
  cursorIndex = 0;
  return result;
};

int main() {
  struct termios terminalSettings;

  cout << "Enter Expression. Type 'exit' to quit." << '\n' << ">  " << flush;

  setNonCanonicalMode(terminalSettings);

  do {
    const StringResult result = newExpression();
    displayResult(result.str);

  } while (input != "exit");

  restoreCanonicalMode(terminalSettings);

  cout << endl << "Successfully exited" << endl;
}
