#include "util.h"
#include "main.h"
#include "devices.h"

#include <iterator>
#include <sstream>

int indicator = 0;

template <typename Out> void split(const string &s, char delim, Out result) {
  std::stringstream ss(s);
  string            item;
  while (std::getline(ss, item, delim)) {
    *(result++) = item;
  }
}

vector<string> split(const string &s, char delim) {
  vector<string> elems;
  split(s, delim, std::back_inserter(elems));
  return elems;
}

void printData() {
  AbstractMotor *     m        = &launcher;
  int                 v        = 0;
  int                 d        = 1;
  float               interval = 1000;
  const char *        c        = ",";
  const unsigned long startT   = millis();

  m->moveVoltage(10000);

  while (true) {
    /*
    if (millis() % (unsigned long)interval) {
      v += d;
      if (abs(v) > 200) {
        d *= -1;
        interval *= .5;
      }
    }
    m->moveVelocity(v);
    */

    // clang-format off
    cout << millis() - startT << c
         << v << c
         << m->getActualVelocity() << c
         << m->getPower() * 10 << c
         << m->getTorque() * 100 << c
         << m->getEfficiency() << c
         << (float)m->getVoltage() / 100.f << c
         << (float)m->getCurrentDraw() / 100.f << c
         << m->getTemperature() * (9/5) + 32 << endl;
    // clang-format on
    delay(50);
  }
}
