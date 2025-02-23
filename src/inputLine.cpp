#include "../include/inputLine.h"

#include <string>

size_t InputLine::getCursorIndex() const { return cursorIndex; }

std::string InputLine::getText() const { return text; }

void InputLine::setText(const std::string &text) {
  this->text = text;
  cursorIndex = text.length();
}

void InputLine::setCursorIndex(const size_t cursorIndex) {
  this->cursorIndex = cursorIndex;
}

void InputLine::erase() {
  if (cursorIndex > text.length()) return;
  --cursorIndex;
  text.erase(text.begin() + cursorIndex);
};

void InputLine::insert(char ch) {
  text.insert(cursorIndex, std::string(1, ch));
  ++cursorIndex;
};

void InputLine::reset() {
  text = "";
  cursorIndex = 0;
}

void InputLine::operator++() {
  ++cursorIndex;
  return;
};

void InputLine::operator--() {
  --cursorIndex;
  return;
};
