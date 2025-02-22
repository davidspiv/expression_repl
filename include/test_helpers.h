#include <limits>

static double maxDouble = std::numeric_limits<double>::max();
static double minDouble = std::numeric_limits<double>::min();

double getRandomDouble(double min = minDouble, double max = maxDouble);

bool Equiv(double a, double b, double accuracy = 1e-6);
