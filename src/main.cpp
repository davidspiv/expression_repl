#ifdef _WIN32
#else
#include <termios.h>
#endif

#include "../include/expression.h"
#include "../include/historyCache.h"
#include "../include/ioHelpers.h"

int main() {
#ifndef _WIN32
  struct termios terminalSettings;
#endif

  HistoryCache historyCache;

  Expression expression;

  std::cout << "Enter Expression. Type 'exit' to quit." << "\n>  "
            << std::flush;

#ifndef _WIN32
  setNonCanonicalMode(terminalSettings);
#endif

  while (expression.getInput() != "exit") {
    char ch;

    // TEST HISTORY
    if (historyCache.empty()) {
      historyCache.addEntry("1*1");
      historyCache.addEntry("2*2");
      historyCache.addEntry("3*3");
    }

    while (readNextChar(ch) && ch != '\n') {
      expression.handleChar(ch, historyCache);
    }

    if (expression.getInputState() == Expression::InputState::INPUT) {
      historyCache.addEntry(expression.getInput());
    }

    historyCache.end();

    expression.displayFinalResult();
  }

#ifndef _WIN32
  restoreCanonicalMode(terminalSettings);
#endif

  std::cout << "\nSuccessfully exited" << std::endl;
}
