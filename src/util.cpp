#include "util.h"

#include <iterator>
#include <sstream>
#include <string>
#include <vector>

template <typename Out> void split(const string &s, char delim, Out result) {
  std::stringstream ss(s);
  string            item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

std::vector<string> split(const string &s, char delim) {
  std::vector<string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

int indicator = 0;
