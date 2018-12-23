#include "devices.h"
#include "main.h"
#include "presets.h"
#include "util.h"

void autonomous();

void opcontrol() {
  // maximum speed in RPM for the drive motors
  static const float dmax = 185;

  // target speed in RPM for the flywheel motors
  float ftarget = 75;
  // flywheel speed
  float fspeed = 0;
  // the commanded drive power values
  float leftCmd, rightCmd;
  // counter for flywheel toggle
  unsigned long fcount = millis();
  // toggle for flywheel pressing buttons
  bool flywheeltoggle = true;
  // flipper position
  int flp;
  // toggle flipper hold
  bool flptoggle;

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

    // if x is pressed, set the drive to hold, and if the commanded power value isn't 0, coast.
    // Otherwise, if the robot is still (or almost still), keep it still by setting a commanded
    // velocity of zero
    if (controller.getDigital(ControllerDigital::X)) {
      left.setBrakeMode(AbstractMotor::brakeMode::hold);
      right.setBrakeMode(AbstractMotor::brakeMode::hold);
    } else if (leftCmd || rightCmd) {
      if (bmode != AbstractMotor::brakeMode::coast) {
        bmode = AbstractMotor::brakeMode::coast;
        left.setBrakeMode(bmode);
        right.setBrakeMode(bmode);
      }
    } else if (abs(left.getActualVelocity()) + abs(right.getActualVelocity()) < 16) {
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
    if (controller.getDigital(ControllerDigital::down)) {
      flipper.moveAbsolute(flpMin, flpSpeed);
      flptoggle = false;
    } else if (controller.getDigital(ControllerDigital::up)) {
      flipper.moveAbsolute(flpMax, flpSpeed);
      flptoggle = false;
    } else {
      if (!flptoggle) {
        flp       = flipper.getPosition();
        flptoggle = true;
      }
      flipper.moveAbsolute(flp, flpSpeed);
    }

    lift.moveVelocity(200 * controller.getDigital(ControllerDigital::L1) -
                      200 * controller.getDigital(ControllerDigital::L2));

    // set the flywheel speed such that if A is pressed, it toggles forward, and if B is pressed, it
    // toggles backward motion. if the either mode is powered on and a button is pressed, the
    // flywheel no longer recieves power
    if (flywheeltoggle) {
      flywheeltoggle = false;
      fcount         = millis();

      if (controller.getDigital(ControllerDigital::A)) {
        fspeed = ftarget;
      } else if (controller.getDigital(ControllerDigital::B)) {
        fspeed = -15;
      } else if (controller.getDigital(ControllerDigital::left)) {
        fspeed = ftarget - 20;
      } else if (controller.getDigital(ControllerDigital::right)) {
        fspeed = ftarget + 25;
      } else {
        flywheeltoggle = true;
      }
    } else {
      if (millis() - fcount >= 250) flywheeltoggle = true;
    }

    launcher.moveVelocity(fspeed); // move the flywheel

    delay(25);
  }
}
