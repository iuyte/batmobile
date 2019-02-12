#ifndef _IUYTE_UTIL_H_
#define _IUYTE_UTIL_H_

#include <string>
#include <utility>
#include <vector>

using std::string;
using std::vector;

#define trim(num, min, max) (num > min ? (num < max ? num : max) : min)
#define waitUntil(condition, loopTime)                                                             \
  while (!(condition))                                                                             \
  pros::delay(loopTime)

vector<string> split(const string &s, char delim);

void infoLoop(void *none);
void chooseAuton();

extern int indicator;

#endif
