#ifndef IO_H
#define IO_H

#ifdef _WIN32

#else
#include <termios.h>
#endif

#include <string>

#include "../include/inputLine.h"
#include "result.h"

#define CLEAR "\033[0K"
#define GREY "\033[38;5;8m"
#define YELLOW "\033[33m"
#define WHITE "\033[0m"
#define PREV_LINE "\033[F"
#define CURSOR_LEFT "\033[1D"
#define CURSOR_RIGHT "\033[1C"

void setNonCanonicalMode(struct termios &initialSettings);
void restoreCanonicalMode(const struct termios &initialSettings);
bool readNextChar(char &ch);
void displayInput(const InputLine &inputLine, const ResultAsString &result);
void displayError(const InputLine &inputLine, const ResultAsString &result);
void displayResult(const std::string &result);

#endif
