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

bool isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) || opRank.count(string(1, ch)) ||
         ch == ')' || ch == ' ';
}

string handleInput(string& input) {
  size_t cursorIndex = 0;
  StringResult result;
  char ch;
  input = "";

  // TEST
  if (historyCache.empty()) {
    historyCache.addEntry("1*1");
    historyCache.addEntry("2*2");
    historyCache.addEntry("3*3");
  }

  while (readNextChar(ch) && ch != '\n') {
    result.str = "";

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

    if (!input.empty()) {
      try {
        TokensResult algResult = lexer(input);
        Token::Type lastTokenType = algResult.tokens.back().getType();

        if (algResult.errMessage.empty() && lastTokenType != Token::BinaryOp &&
            lastTokenType != Token::UnaryOp) {
          TokensResult rpnResult = parser(algResult.tokens);

          if (rpnResult.errMessage.empty()) {
            result = evalRpn(rpnResult.tokens);
          }
        }
      } catch (const std::exception& e) {
      }
    }

    displayInput(input, result, cursorIndex);
  }

  if (historyCache.isEnd() || historyCache.getCurrent() != input) {
    historyCache.addEntry(input);
  }
  historyCache.end();

  return result.str;
};

int main() {
  struct termios terminalSettings;
  string input;

  cout << "Enter Expression. Type 'exit' to quit." << '\n' << ">  " << flush;

  setNonCanonicalMode(terminalSettings);

  while (input != "exit") {
    const string result = handleInput(input);
    displayResult(result);
  }

  restoreCanonicalMode(terminalSettings);

  cout << endl << "Successfully exited" << endl;
}
