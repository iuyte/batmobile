#include "devices.h"
#include "main.h"
#include "presets.h"
#include "util.h"
#include "switcher.h"

void autonomous() {
  // make it so the drive motors hold commanded positions, rather than coast like in driver-control
  left.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
  right.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);

  auton();

  // wait for something to happen
  while (pros::competition::is_autonomous())
    delay(50);
}
