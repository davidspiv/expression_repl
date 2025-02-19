#include <deque>
#include <iostream>
#include <stack>

#include "../include/_math.h"
#include "../include/taylorSeries.h"
#include "../include/token.h"

double evalRpnNotation( std::deque<Token> &rpnNotation) {
  std::stack<double> result;

  for (Token &token : rpnNotation) {
    // handle numerics
    if (token.getType() == Token::Value) {
      result.push(stod(token.getSymbol()));
      continue;
    }

    // handle unary operators
    if (result.empty()) {
      throw std::invalid_argument("invalid expression involving operator \"" +
                                  token.getSymbol() + " \".");
    }
    const double operandB = result.top();
    result.pop();

    if (token.getSymbol() == "_") {
      //   std::cout << "hello" << std::flush;
      result.push(operandB * -1);
      continue;
    }

    if (token.getType() == Token::UnaryOp) {
      result.push(taylorSeries(token, operandB));
      continue;
    }

    // handle binary operators
    if (result.empty()) {
      throw std::invalid_argument("invalid expression involving operator \"" +
                                  token.getSymbol() + "\".");
    }

    const double operandA = result.top();
    result.pop();

    if (token.getSymbol() == "^") {
      result.push(_pow(operandA, operandB));

    } else if (token.getSymbol() == "+") {
      result.push(operandA + operandB);

    } else if (token.getSymbol() == "-") {
      result.push(operandA - operandB);

    } else if (token.getSymbol() == "*") {
      result.push(operandA * operandB);

    } else if (token.getSymbol() == "/") {
      if (!operandB) {
        throw std::invalid_argument("unable to divide by zero.");
      }
      result.push(operandA / operandB);
    }
  }
  return result.top();
}
