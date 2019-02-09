#include "autonomous.h"

vfptr  auton     = &autonSkills;
string autonName = "other->skills";

void autonomous() {
  // make it so the drive motors hold commanded positions, rather than coast like in driver-control
  drive::left.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  drive::right.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

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
    Point{42_in, 0_ft, 0_deg},
  }, "F2");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{8_in, 0_ft, 0_deg},
  }, "F3");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{20_in, 0_ft, 0_deg},
  }, "FC1");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{24_in, 0_ft, 0_deg},
  }, "24in");
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
