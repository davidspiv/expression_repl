#include <deque>
#include <iostream>
#include <string>

#include "../include/evalRpnNotation.h"
#include "../include/historyCache.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/shuntingYard.h"
#include "../include/token.h"
#include "../include/tokensResult.h"

using namespace std;

static HistoryCache historyCache;

bool isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) || opRank.count(string(1, ch)) ||
         ch == ')' || ch == ' ';
}

const string handleInput(string& input) {
  size_t cursorIndex = 0;
  string result;
  char ch;
  input = "";

  while (readNextChar(ch) && ch != '\n') {
    result = "";

    if (ch == '\033') {  // handle ANSI escape sequence

      char escCode[2];

      if (readNextChar(escCode[0]) && readNextChar(escCode[1]) &&
          escCode[0] == '[') {
        switch (escCode[1]) {
          case 'A':
            historyCache.moveBackward();
            input = historyCache.getCurrent();
            cursorIndex = input.length();
            break;

          case 'B':
            historyCache.moveForward();
            input = historyCache.getCurrent();
            cursorIndex = input.length();
            break;
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
    if (input.length()) {
      try {
        deque<Token> algNotation = lexer(input);
        if (!algNotation.empty()) {
          std::deque<Token> rpnNotation = shuntingYard(algNotation);
          result = to_string(evalRpnNotation(rpnNotation));
        }

      } catch (const std::exception& e) {
        //   std::cerr << e.what() << '\n';
      }
    }

    displayInput(input, result, cursorIndex);
  }

  if (historyCache.isBeginning() || historyCache.getCurrent() != input) {
    historyCache.addEntry(input);
  }
  historyCache.beginning();

  return result;
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
