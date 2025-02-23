#include "../include/io.h"

#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "../include/result.h"

using namespace std;

void setNonCanonicalMode(struct termios &initialSettings) {
  struct termios newSettings;
  tcgetattr(STDIN_FILENO, &initialSettings);  // Get current terminal attributes
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

bool readNextChar(char &ch) { return read(STDIN_FILENO, &ch, 1) == 1; }

static bool isSecondLine = false;

void displayInput(const string &input, size_t cursorIndex) {
  ostringstream out;

  out << '\r' << CLEAR << ">  " << input;

  for (size_t i = 0; i < input.size() - cursorIndex; i++) {
    out << CURSOR_LEFT;
  }

  cout << out.str() << flush;
}

void displayTempResult(const StringResult &result,
                       size_t cursorIndex) {
  ostringstream out;

  if (!result.str.empty() && result.errMessage.empty()) {
    out << '\n' << CLEAR << GREY << stod(result.str) << WHITE << PREV_LINE;

    isSecondLine = true;

  } else if (isSecondLine) {
    out << '\n' << CLEAR << PREV_LINE;
  } else {
    return;  // don't display anything if nothings changed
  }

  for (size_t i = 0; i < cursorIndex + 3; i++) {
    out << CURSOR_RIGHT;
  }

  cout << out.str() << flush;
}

void displayResult(const string &result) {
  if (!result.empty()) {
    cout << '\n'
         << YELLOW << CLEAR << stod(result) << WHITE << '\n'
         << ">  " << flush;
  }
  isSecondLine = false;
}
