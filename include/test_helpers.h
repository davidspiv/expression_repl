#include <limits>

static double maxDouble = std::numeric_limits<double>::max();
static double minDouble = std::numeric_limits<double>::min();

double getRandomDouble(double min = minDouble, double max = maxDouble);
