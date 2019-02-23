#include "main.h"

void opcontrol() {
  // auto ballGrabber = Motor(8);
  auto ballGrabber = 8_rmtr;
  ballGrabber.setBrakeMode(AbstractMotor::brakeMode::hold);

  // maximum speed in RPM for the drive motors
  static const float dmax = 200;

  // acceptable time between presses to make a change
  static const unsigned long lpresst = 325;
  // time of last press of flywheel controls
  unsigned long lpress[5];

  // flywheel speed
  float fspeed = 0;
  // the commanded drive power values
  float fwdCmd, turnCmd, strafeCmd;
  // counter for flywheel toggle
  unsigned long fcount = millis();

  // set the drive to "coast" mode, as it is more natural for drivers
  AbstractMotor::brakeMode bmode = AbstractMotor::brakeMode::coast;
  drive::left.setBrakeMode(bmode);
  drive::right.setBrakeMode(bmode);
  drive::dc.setMaxVelocity(dmax);

  // drive position to hold at
  double driveHoldPos[] = {0, 0};
  bool   lastHoldPress  = false;

  // rate to keep loop at constant rate
  Rate rate;

  // infinite driver-control loop, runs: drive, intake, and launcher
  while (true) {
    // get the input control for the drive
    fwdCmd    = controller::get::drive::forward();
    turnCmd   = controller::get::drive::turn();
    strafeCmd = controller::get::drive::strafe();

    drive::control(fwdCmd, turnCmd, strafeCmd);

    // if x is pressed, set the drive to hold, and if the commanded power value isn't 0, coast.
    // Otherwise, if the robot is still (or almost still), keep it still by setting a commanded
    // velocity of zero
    if (controller::get::drive::holdToggle()) {
      if (!lastHoldPress) {
        driveHoldPos[0] = drive::left.getPosition();
        driveHoldPos[1] = drive::right.getPosition();
        lastHoldPress   = true;
      }

      drive::left.moveAbsolute(driveHoldPos[0], dmax);
      drive::right.moveAbsolute(driveHoldPos[1], dmax);
    } else {
      lastHoldPress = false;
      if (fwdCmd || turnCmd || strafeCmd) {
        if (bmode != AbstractMotor::brakeMode::coast) {
          bmode = AbstractMotor::brakeMode::coast;
          drive::left.setBrakeMode(bmode);
          drive::right.setBrakeMode(bmode);
        }
      } else if (abs(drive::left.getActualVelocity()) + abs(drive::right.getActualVelocity()) <
                 16) {
        drive::left.moveVelocity(0);
        drive::right.moveVelocity(0);

        if (bmode != AbstractMotor::brakeMode::hold) {
          bmode = AbstractMotor::brakeMode::hold;
          drive::left.setBrakeMode(bmode);
          drive::right.setBrakeMode(bmode);
        }
      }
    }

    // move the intake based on whether or not the right bumpers are pressed
    intake.move(127 * controller::get::intake());

    switch (controller::get::catapult()) {
    case 1:
      catapult::move();
      break;
    case -1:
      // catapult::motor.moveVelocity(-25);
      break;
    default:
      break;
    }

    if (controller::master.getDigital(ControllerDigital::down)) {
      ballGrabber.moveAbsolute(300, 200);
    } else if (controller::master.getDigital(ControllerDigital::up)) {
      ballGrabber.moveAbsolute(-25, 200);
    } else {
      ballGrabber.moveVelocity(0);
    }

    cout << ballGrabber.getPosition() << endl;

    if constexpr (!atCompetition) {
      if (controller::master.getDigital(ControllerDigital::Y))
        auton();
    }

    delay(25);
    rate.delay(50_Hz);
  }
}
