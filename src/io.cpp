#ifdef _WIN32
#else
#include <termios.h>
#endif

#include <unistd.h>

#include <iostream>
#include <sstream>

#include "../include/inputLine.h"
#include "../include/io.h"
#include "../include/result.h"

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
