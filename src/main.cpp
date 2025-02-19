#include <deque>
#include <iostream>
#include <string>
// #include <exception>

#include "../include/evalRpnNotation.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/shuntingYard.h"
#include "../include/token.h"
#include "../include/tokensResult.h"

using namespace std;

bool isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) || opRank.count(string(1, ch)) ||
         ch == ')';
}

const string handleInput(string& input) {
  size_t cursorIndex = 0;
  string result;
  char ch;
  input = "";

  cout << ">  " << flush;

  while (readNextChar(ch) && ch != '\n') {
    result = "";

    if (ch == '\033') {  // handle ANSI escape sequence

      char escCode[2];

      if (readNextChar(escCode[0]) && readNextChar(escCode[1]) &&
          escCode[0] == '[') {
        switch (escCode[1]) {
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
      continue;

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

  return result;
};

int main() {
  struct termios terminalSettings;
  string input;

  cout << "Enter Expression. Type 'exit' to quit." << '\n';

  setNonCanonicalMode(terminalSettings);

  while (input != "exit") {
    const string result = handleInput(input);
    displayResult(result);
  }

  restoreCanonicalMode(terminalSettings);

  cout << "Successfully exited" << endl;
}
