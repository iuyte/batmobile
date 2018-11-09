#include "presets.h"

// distance that the gear turns in clicks
static const float gDis = 350;
// launcher home position
float lph = 0;
// new and old state variables

void launcherReady() {
  // if the launcher has a high torque, it means that the slip-gear is now driving
  // the gear the catapult is on, and if this is new, then the home position should
  // be reset
  launcher.move(127);
  while (launcher.getTorque() > 1.2 && launcher.getPosition() - lph < 700) {
    delay(25);
  }

  // set the home position to the current position, the beginning of the gear's
  // motion in contact with the arm gear
  lph = launcher.getPosition();
  // pull back the launcher
  // launcher.moveAbsolute(lph + gDis, 200);
  while (launcher.getPosition() < lph + gDis)
    delay(25);

  // hold the launcher arm back
  launcher.moveVelocity(0);
}

void launcherFire() {
  // if the launcher is not all of the way back, make it so
  if (launcher.getPosition() - lph < gDis - 75) {
    launcher.moveAbsolute(lph + gDis, 200);
  }

  // move the slip-gear such that the arm is set in the launching motion
  launcher.move(127);
  delay(225);
  launcher.move(0);
}
