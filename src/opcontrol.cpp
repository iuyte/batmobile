#include "devices.h"
#include "main.h"
#include "presets.h"
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
  // maximum speed in RPM for the drive motors
  static const float dmax = 185;
  // maximum speed in RPM for the flywheel motors
  static const float fmax = 75;

  // flywheel speed
  float fspeed = 0;
  // the commanded drive power values
  float leftCmd, rightCmd;
  // counter for flywheel toggle
  unsigned long fcount = millis();

  // set the drive to "coast" mode, as it is more natural for drivers
  AbstractMotor::brakeMode bmode = AbstractMotor::brakeMode::coast;
  left.setBrakeMode(bmode);
  right.setBrakeMode(bmode);

  // infinite driver-control loop, runs: drive, intake, and launcher
  while (true) {
    // get the input control for the drive
    leftCmd  = controller.getAnalog(ControllerAnalog::leftY) * dmax;
    rightCmd = controller.getAnalog(ControllerAnalog::rightY) * dmax;

    left.moveVelocity(leftCmd);
    right.moveVelocity(rightCmd);

    // if the commanded power value isn't 0, coast. Otherwise, if the robot is still, keep it
    // still by setting a commanded velocity of zero
    if (leftCmd || rightCmd) {
      if (bmode != AbstractMotor::brakeMode::coast) {
        bmode = AbstractMotor::brakeMode::coast;
        left.setBrakeMode(bmode);
        right.setBrakeMode(bmode);
      }
    } else if (!(left.getActualVelocity() + right.getActualVelocity())) {
      left.moveVelocity(0);
      right.moveVelocity(0);

      if (bmode != AbstractMotor::brakeMode::hold) {
        bmode = AbstractMotor::brakeMode::hold;
        left.setBrakeMode(bmode);
        right.setBrakeMode(bmode);
      }
    }

    intake.move(127 * controller.getDigital(ControllerDigital::R1) -
                127 * controller.getDigital(ControllerDigital::R2));
    flipper.moveAbsolute(
            controller.getDigital(ControllerDigital::down)
                    ? 0
                    : (controller.getDigital(ControllerDigital::up) ? 500 : flipper.getPosition()),
            135);
    lift.moveVelocity(200 * controller.getDigital(ControllerDigital::L1) -
                      200 * controller.getDigital(ControllerDigital::L2));

    // set the flywheel speed such that if A is pressed, it toggles forward, and if B is pressed, it
    // toggles backward motion. if the either mode is powered on and a button is pressed, the
    // flywheel no longer recieves power
    if (millis() - fcount > 250) {
      fspeed = controller.getDigital(ControllerDigital::A)
                       ? ((fspeed) ? 0 : fmax)
                       : (controller.getDigital(ControllerDigital::B) ? ((fspeed) ? 0 : -15)
                                                                      : fspeed);
      fcount = millis();
    }
    launcher.moveVelocity(fspeed); // move the flywheel

    delay(25);
  }
}
