#include "devices.h"
#include "main.h"
#include "util.h"

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
  // The maximum speed in RPM for the drive motors
  const float dmax = 185;
  // Set the left and right to be more natural for drivers
  okapi::AbstractMotor::brakeMode bmode = okapi::AbstractMotor::brakeMode::coast;
  left.setBrakeMode(bmode);
  right.setBrakeMode(bmode);

  while (true) {
    left.moveVelocity(controller.getAnalog(okapi::ControllerAnalog::leftY) * dmax +
                      controller.getDigital(okapi::ControllerDigital::up) * dmax -
                      controller.getDigital(okapi::ControllerDigital::down) * dmax);
    right.moveVelocity(controller.getAnalog(okapi::ControllerAnalog::rightY) * dmax +
                       controller.getDigital(okapi::ControllerDigital::up) * dmax -
                       controller.getDigital(okapi::ControllerDigital::down) * dmax);

    intake.move(127 * controller.getDigital(okapi::ControllerDigital::R1) +
                -127 * controller.getDigital(okapi::ControllerDigital::R2));
    launcher.move(127 * controller.getDigital(okapi::ControllerDigital::R1) +
                  -127 * controller.getDigital(okapi::ControllerDigital::R2));

    pros::delay(20);
  }
}
