#ifdef _WIN32
#else
#include <termios.h>
#endif

#include "../include/expression.h"
#include "../include/historyCache.h"
#include "../include/io.h"

int main() {
#ifndef _WIN32
  struct termios terminalSettings;
#endif

  InputLine inputLine;

  std::cout << "Enter Expression. Type 'exit' to quit." << "\n>  "
            << std::flush;

#ifndef _WIN32
  setNonCanonicalMode(terminalSettings);
#endif

  do {
    const ResultAsString result = newExpression(inputLine);
    displayResult(result.str);

  } while (inputLine.getText() != "exit");

#ifndef _WIN32
  restoreCanonicalMode(terminalSettings);
#endif

  std::cout << "\nSuccessfully exited" << std::endl;
}
