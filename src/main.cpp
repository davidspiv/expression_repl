#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/token.h"
#include "../include/tokensResult.h"

using namespace std;

bool isDisplayable(char ch) {
  return isNumeric(ch) || isalpha(ch) ||
         opRank.find(string(1, ch)) != opRank.end();
}

void getInput(string &input) {
  char ch;
  size_t cursorIndex = 0;
  vector<Token> algNotation;

  input = "";

  displayInput(input, input, cursorIndex);

  while (readNextChar(ch) && ch != '\n') {
    string freshInput = "";
    int targetIndex = algNotation.size() - 1;

    int charCount = input.length() - cursorIndex;

    // Token tokenA;
    // Token tokenB;

    while (charCount > 0) {
      charCount -= algNotation.at(targetIndex).length();

      if (charCount > 0) {
        targetIndex--;
      }
    }

    if (ch == '\033') {  // Handle ANSI escape codes
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
    }

    if (ch == 0x7F && cursorIndex) {  // Handle backspace

      size_t length = algNotation.at(targetIndex).length();

      cout << "\b \b";
      cursorIndex--;
      input.pop_back();

      if (length > 1) {
        freshInput =
            algNotation.at(targetIndex).getSymbol().substr(0, length - 1);
      }

      algNotation.erase(algNotation.begin() +
                        targetIndex);  // remove deleted token

      if (freshInput.empty() && !algNotation.empty()) {
        freshInput = algNotation.at(targetIndex - 1).getSymbol();

        algNotation.pop_back();  // remove tokens touching cursor
      }

    } else if (isDisplayable(ch)) {  // handle displayable character

      input.insert(cursorIndex, string(1, ch));
      if (!algNotation.empty()) {
        freshInput += algNotation.at(targetIndex).getSymbol();
        algNotation.erase(algNotation.begin() + targetIndex);
      }
      charCount++;
      if (charCount < 0) {
        freshInput.insert(freshInput.end() + charCount, input.at(cursorIndex));
      } else {
        freshInput.push_back(input.at(cursorIndex));
      }
      cursorIndex++;
    }

    const TokensResult tokensResult = lexer(freshInput);

    for (size_t i = 0; i < tokensResult.tokens.size(); i++) {
      algNotation.push_back(tokensResult.tokens[i]);
    }

    displayInput(input, freshInput, cursorIndex);
  }
};

int main() {
  struct termios terminalSettings;
  string input;

  cout << "Enter Expression. Type 'exit' to quit." << endl;

  setNonCanonicalMode(terminalSettings);

  while (input != "exit") {
    getInput(input);
    displayResult(input);
  }

  restoreCanonicalMode(terminalSettings);

  cout << "Successfully exited" << endl;
}
