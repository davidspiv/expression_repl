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
  return isNumeric(ch) || isalpha(ch) || opRank.count(string(1, ch));
}

string getInput() {
  vector<Token> algNotation;
  size_t cursorIndex = 0;
  string input = "";
  char ch;

  while (readNextChar(ch) && ch != '\n') {
    string freshInput = "";
    int targetIndex = algNotation.size() - 1;
    int charCount = input.length() - cursorIndex;
    bool atTokenStart = false;

    // Token tokenA;
    // Token tokenB;

    while (charCount > 0) {
      charCount -= algNotation.at(targetIndex).length();

      if (charCount > 0) {
        targetIndex--;
      } else {
        atTokenStart = true;
      }
    }

    if (ch == '\033') {
      // HANDLE ANSI ESCAPE SEQUENCE

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
    } else if (ch == 0x7F && cursorIndex) {
      // HANDLE BACKSPACE

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

    } else if (isDisplayable(ch)) {
      // HANDLE DISPLAYABLE CHARACTER

      input.insert(cursorIndex, string(1, ch));
      if (!algNotation.empty()) {
        freshInput += algNotation.at(targetIndex).getSymbol();
        algNotation.erase(algNotation.begin() + targetIndex);
      }

      if (charCount || atTokenStart) {
        freshInput.insert(freshInput.begin() - charCount,
                          input.at(cursorIndex));
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
  return input;
};

int main() {
  struct termios terminalSettings;
  string input;

  cout << "Enter Expression. Type 'exit' to quit." << '\n' << ">  " << flush;

  setNonCanonicalMode(terminalSettings);

  while (input != "exit") {
    input = getInput();
    displayResult(input);
  }

  restoreCanonicalMode(terminalSettings);

  cout << "Successfully exited" << endl;
}
