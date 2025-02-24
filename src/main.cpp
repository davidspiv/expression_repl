#ifdef _WIN32
#else
#include <termios.h>
#endif

#include "../include/evalRpn.h"
#include "../include/expression.h"
#include "../include/historyCache.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/token.h"

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

      Token::Type lastTokenType = algResult.tokens.back().getType();

      if (lastTokenType == Token::UnaryOp || lastTokenType == Token::BinaryOp) {
        expression.setError("hanging operator");
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

    historyCache.end();
    displayResult(expression);
  }

  restoreCanonicalMode(terminalSettings);  // Unix only

  std::cout << "\nSuccessfully exited" << std::endl;
}
