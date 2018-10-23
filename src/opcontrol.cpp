#include "devices.h"
#include "main.h"

void autonomous();

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  float l, r;
  left.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  right.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);

  while (true) {
    l = controller.getAnalog(okapi::ControllerAnalog::leftY) * 200.f;
    r = controller.getAnalog(okapi::ControllerAnalog::rightY) * 200.f;
    left.moveVelocity(l);
    right.moveVelocity(r);

    launcher.moveVelocity(
        200 * controller.getDigital(okapi::ControllerDigital::up) +
        -10 * controller.getDigital(okapi::ControllerDigital::down));

    pros::delay(20);
  }
}
