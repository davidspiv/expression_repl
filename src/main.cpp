#ifdef _WIN32
#else
#include <termios.h>
#endif

#include "../include/expression.h"
#include "../include/ioHelpers.h"

int main() {
#ifndef _WIN32
  struct termios terminalSettings;
#endif

  Expression inputLine;

  std::cout << "Enter Expression. Type 'exit' to quit." << "\n>  "
            << std::flush;

#ifndef _WIN32
  setNonCanonicalMode(terminalSettings);
#endif

  while (inputLine.getInput() != "exit") {
    inputLine.createExpression();
    inputLine.displayFinalResult();
  }

#ifndef _WIN32
  restoreCanonicalMode(terminalSettings);
#endif

  std::cout << "\nSuccessfully exited" << std::endl;
}
