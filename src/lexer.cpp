#include "../include/lexer.h"

#include <deque>
#include <iostream>
#include <string>

#include "../include/token.h"

bool isNumeric(const char symbol) { return isdigit(symbol) || symbol == '.'; }

// Determine if '-' represent negation or subtraction
bool isNegateOp(std::deque<Token> &tokens) {
  return tokens.empty() || (tokens.back().getType() != Token::Value &&
                            tokens.back().getSymbol() != ")");
}

std::deque<Token> lexer(const std::string &input) {
  std::deque<Token> tokens;
  std::string valueBuff = "";
  std::string opSymbolBuff = "";

  for (char symbol : input) {
    const std::string symbolAsString = std::string(1, symbol);

    // handle numeric buffer for multi-char values
    if (isNumeric(symbol)) {
      valueBuff += symbol;
      continue;
    }

    // create value token
    if (!valueBuff.empty()) {
      tokens.push_back(Token(valueBuff, Token::Value));
      valueBuff.clear();
    }

    // handle operator buffer for multi-char names
    if (isalpha(symbol)) {
      opSymbolBuff += symbol;
      if (opSymbolBuff.length() < 3) {
        continue;
      }
      if (!opRank.count(opSymbolBuff)) {
        throw std::invalid_argument("unrecognized function \"" + opSymbolBuff +
                                    "\".");
      }
    }

    // create non-numeric token
    if (!opSymbolBuff.empty()) {
      tokens.push_back(Token(opSymbolBuff, Token::UnaryOp));
      opSymbolBuff.clear();

    } else if (symbol == '(' || symbol == ')') {
      tokens.push_back(Token(symbolAsString, Token::Parenthesis));

    } else if (symbol == '-' && isNegateOp(tokens)) {
      tokens.push_back(Token("_", Token::UnaryOp));

    } else if (symbol == '-') {
      tokens.push_back(Token(symbolAsString, Token::BinaryOp));

    } else if (opRank.find(symbolAsString) != opRank.end()) {
      tokens.push_back(Token(symbolAsString, Token::BinaryOp));

    } else if (symbol != ' ') {
      throw std::invalid_argument("unrecognized symbol \"" + symbolAsString +
                                  "\".");
    }
  }

  if (valueBuff.length()) {
    tokens.push_back(Token(valueBuff, Token::Value));
  }

  return tokens;
}
