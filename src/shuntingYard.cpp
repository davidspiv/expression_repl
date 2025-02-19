#include <deque>
#include <iostream>
#include <stack>

#include "../include/_math.h"
#include "../include/token.h"

size_t safePeekTop(std::stack<Token> &opStack) {
  if (!opStack.empty()) {
    return opRank.at(opStack.top().getSymbol());
  }
  return 0;
}

std::deque<Token> shuntingYard(std::deque<Token> inputQueue) {
  std::stack<Token> opStack;
  std::deque<Token> outputQueue;

  for (Token &token : inputQueue) {
    // std::cout << "TOKEN: " << token.getSymbol() << std::endl;
    if (token.getType() == Token::Value) {
      outputQueue.push_back(token);
      continue;
    }

    // flush opStack until matching parenthesis is reached
    if (token.getSymbol() == ")") {
      if (opStack.empty()) {
        throw std::invalid_argument("mismatched parentheses.");
      }
      while (opStack.top().getSymbol() != "(") {
        outputQueue.push_back(opStack.top());
        opStack.pop();
        if (opStack.empty()) {
          throw std::invalid_argument("mismatched parentheses.");
        }
      }
      opStack.pop();  // remove open parenthesis
      continue;
    }

    const size_t currentOpRank = opRank.at(token.getSymbol());
    size_t previousOpRank = safePeekTop(opStack);

    while (token.getSymbol() != "(" && currentOpRank <= previousOpRank) {
      // nested exponents are unique - must be evaluated right to left
      if (token.getSymbol() == "^" && currentOpRank == previousOpRank) {
        break;
      }

      outputQueue.push_back(opStack.top());
      opStack.pop();
      previousOpRank = safePeekTop(opStack);
    }

    opStack.push(token);
  }

  // flush remaining operators
  while (!opStack.empty()) {
    if (opStack.top().getType() == Token::Parenthesis) {
      throw std::invalid_argument("mismatched parentheses.");
    }
    outputQueue.push_back(opStack.top());
    opStack.pop();
  }

  return outputQueue;
}
