#include "main.h"

void generatePaths();

void initialize() {
  static bool generated = false;

  // Clear controller
  controller::master.clear();

  // reset sensors
  drive::reset();

  // set intake to "coast" mode and catapult/arm to "hold" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  catapult::motor.setBrakeMode(AbstractMotor::brakeMode::hold);
  arm.setBrakeMode(AbstractMotor::brakeMode::hold);

  // start task to handle catapult
  catapult::startManager();

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(&infoLoop, (void *)(&generated));

  delay(50);

  // generate paths to be used in autonomous unless X is pressed
  if (!controller::master.getDigital(ControllerDigital::X))
    generatePaths();

  generated = true;
}

void disabled() {}

void competition_initialize() {}
