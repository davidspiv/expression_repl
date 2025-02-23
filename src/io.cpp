#include "../include/io.h"

#ifdef _WIN32
#else
#include <termios.h>
#endif

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "../include/inputLine.h"
#include "../include/result.h"

static bool isSecondLine = false;

#ifdef __linux__
void setNonCanonicalMode(struct termios &initialSettings) {
  struct termios newSettings;
  tcgetattr(STDIN_FILENO,
            &initialSettings);  // Get current terminal attributes
  newSettings = initialSettings;

  newSettings.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
  newSettings.c_cc[VMIN] = 1;               // Read one character at a time
  newSettings.c_cc[VTIME] = 0;              // No timeout

  tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);  // Apply new settings
}

void restoreCanonicalMode(const struct termios &initialSettings) {
  tcsetattr(STDIN_FILENO, TCSANOW,
            &initialSettings);  // Restore initial settings
}
#endif

bool readNextChar(char &ch) { return read(STDIN_FILENO, &ch, 1) == 1; }

void displayInput(const InputLine &inputLine, const ResultAsString &result) {
  std::ostringstream out;

  if (!result.str.empty() && result.errMessage.empty()) {
    out << '\n' << CLEAR << GREY << stod(result.str) << WHITE << PREV_LINE;

    isSecondLine = true;

  } else if (isSecondLine) {
    out << '\n' << CLEAR << PREV_LINE;
  }

  out << '\r' << CLEAR << ">  " << inputLine.getText();

  for (size_t i = 0;
       i < inputLine.getText().length() - inputLine.getCursorIndex(); i++) {
    out << CURSOR_LEFT;
  }

  std::cout << out.str() << std::flush;
}

void displayError(const InputLine &inputLine, const ResultAsString &result) {
  std::ostringstream out;

  out << '\n' << CLEAR << GREY << result.errMessage << WHITE << PREV_LINE;

  for (size_t i = 0; i < inputLine.getCursorIndex() + 3; i++) {
    out << CURSOR_RIGHT;
  }

  std::cout << out.str() << std::flush;
}

void displayResult(const std::string &result) {
  if (!result.empty()) {
    std::cout << '\n'
              << YELLOW << CLEAR << stod(result) << WHITE << '\n'
              << ">  " << std::flush;
  }
  isSecondLine = false;
}
