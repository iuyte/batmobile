#include "main.h"

void generatePaths();

void initialize() {
  // Clear controller
  controller::master.clear();

  // set flywheel and intake to "coast" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  catapult::motor.setBrakeMode(AbstractMotor::brakeMode::hold);

  // start task to handle catapult
  catapult::startManager();

  // generate paths to be used in autonomous
  generatePaths();

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(&infoLoop, nullptr);

  // reset sensors
  drive::reset();

}

void disabled() {}

void competition_initialize() {}
