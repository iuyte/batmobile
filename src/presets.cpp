#include "presets.h"

// distance that the gear turns in clicks
static const float gDis = 675;
// launcher home position
float lph = 0;
// new and old state variables

void launcherReady() {
  // move the gear until the arm is ready to fire
  launcher.move(127);
  while (pot.get() < gDis)
    delay(25);

  launcher.moveVelocity(0);
}

void launcherFire() {
  // move the slip-gear such that the arm is set in the launching motion
  launcher.move(127);
  delay(225);
  launcher.move(0);
}
