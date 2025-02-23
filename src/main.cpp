#include <deque>
#include <iostream>
#include <string>

#include "../include/evalRpn.h"
#include "../include/expression.h"
#include "../include/historyCache.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/result.h"
#include "../include/token.h"

using namespace std;

int main() {
  struct termios terminalSettings;
  InputLine inputLine;

  cout << "Enter Expression. Type 'exit' to quit." << '\n' << ">  " << flush;

  setNonCanonicalMode(terminalSettings);

  do {
    const ResultAsString result = newExpression(inputLine);
    displayResult(result.str);

  } while (inputLine.getText() != "exit");

  restoreCanonicalMode(terminalSettings);

  cout << endl << "Successfully exited" << endl;
}
