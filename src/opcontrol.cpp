#include "devices.h"
#include "main.h"
#include "presets.h"
#include "switcher.h"
#include "util.h"

void opcontrol() {
  // maximum speed in RPM for the drive motors
  static const float dmax = 185;

  // low/high flag flywheel speeds, respectively
  const float fpreset[2] = {94, 122};
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
  int flp = flpMin;
  // toggle flipper hold
  bool flptoggle = false;

  // set the drive to "coast" mode, as it is more natural for drivers
  AbstractMotor::brakeMode bmode = AbstractMotor::brakeMode::coast;
  left.setBrakeMode(bmode);
  right.setBrakeMode(bmode);

  if (controller::master.getDigital(ControllerDigital::X)) auton();

  // infinite driver-control loop, runs: drive, intake, and launcher
  while (true) {
    // get the input control for the drive
    leftCmd  = controller::leftDrive() * dmax;
    rightCmd = controller::rightDrive() * dmax;

    left.moveVelocity(leftCmd);
    right.moveVelocity(rightCmd);

    // if x is pressed, set the drive to hold, and if the commanded power value isn't 0, coast.
    // Otherwise, if the robot is still (or almost still), keep it still by setting a commanded
    // velocity of zero
    if (controller::driveHoldToggle()) {
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

    // move the lift based on whether or not the left bumpers are pressed
    lift.moveVelocity(185 * controller::lift());

    // move the intake based on whether or not the right bumpers are pressed
    intake.move(127 * controller::intake());

    // move the flipper based on whether or not the up/down buttons are pressed
    if (controller::flipper() < 0) {
      flipper.moveAbsolute(flpMin, flpSpeed);
      flptoggle = false;
    } else if (controller::flipper() > 0) {
      flipper.moveAbsolute(flpMax, flpSpeed);
      flptoggle = false;
    } else {
      // if neither button is pressed, hold the position
      if (!flptoggle) {
        flp       = flipper.getPosition();
        flptoggle = true;
      }

      // if the lift is above a low height and the flipper is low, angle it up a bit to prevent a
      // cap from falling off
      if (lift.getPosition() < lift_ing) {
        if (flp == flpHold) flp = flpMin;
      } else if (flp < flpHold) {
        flp = flpHold;
      }

      flipper.moveAbsolute(flp, flpSpeed);
    }

    if (controller::launcher::toggle() && millis() - fcount > 250) {
      fspeed = (flywheeltoggle = !flywheeltoggle) ? ftarget : 0;
      fcount = millis();
    } else if (controller::launcher::backwards()) {
      fspeed = -15;
    } else if (controller::launcher::less()) {
      fspeed = ftarget -= 2.5;
    } else if (controller::launcher::more()) {
      fspeed = ftarget += 2.5;
    } else if (controller::launcher::middleFlag()) {
      ftarget = fpreset[LOW];
      fspeed  = fpreset[LOW];
    } else if (controller::launcher::highFlag()) {
      ftarget = fpreset[HIGH];
      fspeed  = fpreset[HIGH];
    }

    launcher.moveVelocity(fspeed); // move the flywheel

    delay(25);
  }
}
