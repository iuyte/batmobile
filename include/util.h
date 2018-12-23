#ifndef _IUYTE_UTIL_H_
#define _IUYTE_UTIL_H_

#include "main.h"

#define trim(num, min, max) (num > min ? (num < max ? num : max) : min)

std::vector<string> split(const string &s, char delim);

extern int indicator;

#endif
