#ifndef _IUYTE_UTIL_H_
#define _IUYTE_UTIL_H_

#include <cmath>
#include <string>
#include <utility>
#include <vector>

#include "main.h"

using std::string;
using std::vector;

#define trim(num, min, max) (num > min ? (num < max ? num : max) : min)
#define waitUntil(condition, loopTime)                                                             \
  while (!(condition))                                                                             \
  pros::delay(loopTime)

#define waitUntiloD(condition, loopTime)                                                           \
  while (!(condition || pros::competition::is_disabled()))                                         \
  pros::delay(loopTime)

vector<string> split(const string &s, char delim);

void infoLoop(void *none);
void printData();

extern int indicator;

static const double sqrt2 = sqrt(2);

/** convert degrees of rotations to motor ticks */
// inline double dt(double deg, int tpr = imev5GreenTPR) { return deg * (float)tpr * (1.f/360.f); }

#endif
