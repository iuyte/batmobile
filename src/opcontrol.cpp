#include "main.h"

void opcontrol() {
  // maximum speed in RPM for the drive motors
  static const float dmax = 190;

  // acceptable time between presses to make a change
  static const unsigned long lpresst = 325;
  // time of last press of flywheel controls
  unsigned long lpress[5];

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
  drive::left.setBrakeMode(bmode);
  drive::right.setBrakeMode(bmode);
  drive::dc.setMaxVelocity(200);

  // infinite driver-control loop, runs: drive, intake, and launcher
  while (true) {
    // get the input control for the drive
    leftCmd  = controller::leftDrive();
    rightCmd = controller::rightDrive();

    drive::dc.tank(leftCmd, rightCmd, .05);

    // if x is pressed, set the drive to hold, and if the commanded power value isn't 0, coast.
    // Otherwise, if the robot is still (or almost still), keep it still by setting a commanded
    // velocity of zero
    if (controller::driveHoldToggle()) {
      drive::left.setBrakeMode(AbstractMotor::brakeMode::hold);
      drive::right.setBrakeMode(AbstractMotor::brakeMode::hold);
    } else if (leftCmd || rightCmd) {
      if (bmode != AbstractMotor::brakeMode::coast) {
        bmode = AbstractMotor::brakeMode::coast;
        drive::left.setBrakeMode(bmode);
        drive::right.setBrakeMode(bmode);
      }
    } else if (abs(drive::left.getActualVelocity()) + abs(drive::right.getActualVelocity()) < 16) {
      drive::left.moveVelocity(0);
      drive::right.moveVelocity(0);

      if (bmode != AbstractMotor::brakeMode::hold) {
        bmode = AbstractMotor::brakeMode::hold;
        drive::left.setBrakeMode(bmode);
        drive::right.setBrakeMode(bmode);
      }
    }

    // move the lift based on whether or not it is at maximum/minimum height
    if (lift.getPosition() < lift_bottom) {
      lift.moveVelocity(trim(liftSpeed * controller::lift(), 0, liftSpeed));
    } else if (lift.getPosition() > lift_top) {
      lift.moveVelocity(trim(liftSpeed * controller::lift(), -liftSpeed, 0));
    } else {
      lift.moveVelocity(liftSpeed * controller::lift());
    }

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

    if (controller::launcher::off()) {
      fspeed = 0;
    } else if (controller::launcher::backwards() && millis() - lpress[0] > lpresst) {
      fspeed = fspeed ? 0 : -15;
      lpress[0] = millis();
    } else if (controller::launcher::less() && millis() - lpress[1] > lpresst) {
      fspeed -= 2.5;
      lpress[1] = millis();
    } else if (controller::launcher::more() && millis() - lpress[2] > lpresst) {
      fspeed += 2.5;
      lpress[2] = millis();
    } else if (controller::launcher::middleFlag() && millis() - lpress[3] > lpresst) {
      fspeed  = (fspeed == fpreset[LOW]) ? 0 : fpreset[LOW];
      lpress[3] = millis();
    } else if (controller::launcher::highFlag() && millis() - lpress[4] > lpresst) {
      fspeed  = (fspeed == fpreset[HIGH]) ? 0 : fpreset[HIGH];
      lpress[4] = millis();
    }

    launcher.moveVelocity(fspeed); // move the flywheel

    if (controller::master.getDigital(ControllerDigital::X)) auton();

    delay(25);
  }
}
