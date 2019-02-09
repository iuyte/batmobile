#include "main.h"

void generatePaths();

void initialize() {
  // Clear controller
  controller::master.clear();

  // set flywheel and intake to "coast" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  catapult.setBrakeMode(AbstractMotor::brakeMode::hold);

  // task to handle catapult
  static pros::Task catapultTaskHandleL(&catapultTask, nullptr);
  catapultTaskHandle = &catapultTaskHandleL;

  // generate paths to be used in autonomous
  generatePaths();

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(&infoLoop, nullptr);

}

void disabled() {}

void competition_initialize() {}
