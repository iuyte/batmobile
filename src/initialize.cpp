#include "main.h"

bool isSelected = false;

void initialize() {
  // Clear controller
  controller::master.clear();
  controller::partner.clear();

  // set flywheel and intake to "coast" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  launcher.setBrakeMode(AbstractMotor::brakeMode::coast);
  // set the lift to "hold" mode
  lift.setBrakeMode(AbstractMotor::brakeMode::hold);

  // set the flywheel pid
  launcher.setVelPID(0, 2, 0, 7);

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(&infoLoop, nullptr);
}

void disabled() {}

void competition_initialize() {}
