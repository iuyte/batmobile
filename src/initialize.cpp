#include "main.h"

bool isSelected = false;

void initialize() {
  // Clear controller
  controller::master.clear();

  // set flywheel and intake to "coast" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  catapult.setBrakeMode(AbstractMotor::brakeMode::hold);

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(&infoLoop, nullptr);

  // task to handle catapult
  static pros::Task catapultTaskHandleL(&catapultTask, nullptr);
  catapultTaskHandle = &catapultTaskHandleL;
}

void disabled() {}

void competition_initialize() {}
