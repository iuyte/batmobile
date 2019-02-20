#include "autonomous.h"

vfptr  auton     = &autonSkills2;
string autonName = "other->skills->new";

void autonomous() {
  // make it so the drive motors hold commanded positions, rather than coast like in driver-control
  drive::left.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  drive::right.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

  // reset sensors
  drive::reset();

  cout << "Starting autonomous... ";

  // run the selected autonomous
  auton();

  cout << "Done!" << endl;

  // wait for something to happen
  while (pros::competition::is_autonomous())
    delay(50);
}

void generatePaths() {
  // clang-format off
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{38_in, 0_ft, 0_deg},
  }, "F1");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{34_in, 0_ft, 0_deg},
  }, "F2");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{12_in, 0_ft, 0_deg},
  }, "F3");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{18_in, 0_ft, 0_deg},
  }, "FC1");
  // clang-format on
}

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
