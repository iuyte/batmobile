#include "autonomous.h"

vfptr  auton     = &autonSkills;
string autonName = "skills";

void autonomous() {
  // make it so the drive motors hold commanded positions, rather than coast like in driver-control
  drive::left.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
  drive::right.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);

  // enable the controller
  // dc.flipDisable(false);

  cout << "Starting autonomous... ";

  // run the selected autonomous
  auton();

  cout << "Done!" << endl;

  // disable the controller
  drive::dpc.flipDisable();

  // wait for something to happen
  while (pros::competition::is_autonomous())
    delay(50);
}

void generatePaths() {}

void moveToRelativePos(Motor         motor,
                       double        position,
                       double        velocity,
                       double        errorTolerance,
                       unsigned long loopTime) {
  motor.moveRelative(position, velocity);
  waitUntil(motorPosTargetReached(intake, errorTolerance), loopTime);
}

void moveToAbsolutePos(Motor         motor,
                       double        position,
                       double        velocity,
                       double        errorTolerance,
                       unsigned long loopTime) {
  motor.moveAbsolute(position, velocity);
  waitUntil(motorPosTargetReached(intake, errorTolerance), loopTime);
}
