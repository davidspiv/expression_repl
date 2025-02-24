#ifdef _WIN32
#else
#include <termios.h>
#endif

#include "../include/c_expression.h"
#include "../include/c_historyCache.h"
#include "../include/c_token.h"
#include "../include/evalRpn.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/parser.h"

int main() {
  struct termios terminalSettings;
  HistoryCache historyCache;
  Expression expression;

  setNonCanonicalMode(terminalSettings);  // Unix only

  std::cout << "Enter Expression. Type 'exit' to quit." << "\n>  "
            << std::flush;

  while (expression.getInput() != "exit") {
    char ch;

    // TEST HISTORY
    if (historyCache.empty()) {
      historyCache.addEntry("1*1");
      historyCache.addEntry("2*2");
      historyCache.addEntry("3*3");
    }

    while (readNextChar(ch) && ch != '\n') {
      if (!expression.handleChar(ch, historyCache)) continue;

      if (expression.getInput().empty()) {
        expression.setError("empty input");
        updateDisplay(expression);
        continue;
      }

      ResultAsTokens algResult = lexer(expression.getInput());

      if (!algResult.errMessage.empty()) {
        expression.setError(algResult.errMessage);
        updateDisplay(expression);
        continue;
      }

      ResultAsTokens rpnResult = parser(algResult.tokens);

      if (!rpnResult.errMessage.empty()) {
        expression.setError(rpnResult.errMessage);
        updateDisplay(expression);
        continue;
      }

      ResultAsString resultAsString = evalRpn(rpnResult.tokens);
      expression.setResult(resultAsString.str);
      expression.setError(resultAsString.errMessage);
      updateDisplay(expression);
    }

    if (expression.getInputState() == Expression::InputState::INPUT) {
      historyCache.addEntry(expression.getInput());
    }

    displayResult(expression);
    historyCache.end();
  }

  restoreCanonicalMode(terminalSettings);  // Unix only

  std::cout << "\nSuccessfully exited" << std::endl;
}
