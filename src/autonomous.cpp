#include "autonomous.h"

// vfptr  auton     = &autonSkills2;
// string autonName = "other->skills->new";
vfptr  auton     = &autonTest;
string autonName = "other->test";

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
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{39_in, 0_ft, 0_deg},
  }, "F1");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{33.25_in, 0_ft, 0_deg},
  }, "F2");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{12_in, 0_ft, 0_deg},
  }, "F3");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{18_in, 0_ft, 0_deg},
  }, "FC1");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{2_in, 0_ft, 0_deg},
  }, "2");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{4_in, 0_ft, 0_deg},
  }, "4");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{6_in, 0_ft, 0_deg},
  }, "6");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{6.5_in, 0_ft, 0_deg},
  }, "7");
  drive::dpc->generatePath({
      Point{0_ft, 0_ft, 0_deg},
      Point{8_in, 0_ft, 0_deg},
  }, "8");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{10_in, 0_ft, 0_deg},
  }, "10");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{12_in, 0_ft, 0_deg},
  }, "12");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{14_in, 0_ft, 0_deg},
  }, "14");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{15_in, 0_ft, 0_deg},
  }, "15");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{16_in, 0_ft, 0_deg},
  }, "16");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{18_in, 0_ft, 0_deg},
  }, "18");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{24_in, 0_ft, 0_deg},
  }, "24");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{26_in, 0_ft, 0_deg},
  }, "26");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{30_in, 0_ft, 0_deg},
  }, "30");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{34_in, 0_ft, 0_deg},
  }, "34");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{36_in, 0_ft, 0_deg},
  }, "36");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{40_in, 0_ft, 0_deg},
  }, "40");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{42_in, 0_ft, 0_deg},
  }, "42");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{44_in, 0_ft, 0_deg},
  }, "44");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{48_in, 0_ft, 0_deg},
  }, "48");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{50_in, 0_ft, 0_deg},
  }, "50");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{54_in, 0_ft, 0_deg},
  }, "54");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{60_in, 0_ft, 0_deg},
  }, "60");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{84_in, 0_ft, 0_deg},
  }, "84");
  drive::dpc->generatePath({
    Point{0_ft, 0_ft, 0_deg},
    Point{90_in, 0_ft, 0_deg},
  }, "90");
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
