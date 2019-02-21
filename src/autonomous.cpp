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
    Point{39_in, 0_ft, 0_deg},
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
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{12_in, 0_ft, 0_deg},
  }, "12");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{18_in, 0_ft, 0_deg},
  }, "18");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{24_in, 0_ft, 0_deg},
  }, "24");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{30_in, 0_ft, 0_deg},
  }, "30");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{36_in, 0_ft, 0_deg},
  }, "36");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{42_in, 0_ft, 0_deg},
  }, "42");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{48_in, 0_ft, 0_deg},
  }, "48");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{50_in, 0_ft, 0_deg},
  }, "50");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{54_in, 0_ft, 0_deg},
  }, "54");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{60_in, 0_ft, 0_deg},
  }, "60");
  drive::dpc.generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{84_in, 0_ft, 0_deg},
  }, "84");
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
