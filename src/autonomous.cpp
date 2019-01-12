#include "autonomous.h"

void autonomous() {
  // make it so the drive motors hold commanded positions, rather than coast like in driver-control
  left.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
  right.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);

  // enable the controller
  // dc.flipDisable(false);

  cout << "Starting autonomous... ";

  // run the selected autonomous
  auton();

  cout << "Done!" << endl;

  // disable the controller
  dc.flipDisable();

  // wait for something to happen
  while (pros::competition::is_autonomous())
    delay(50);
}

void generatePaths() {
  //// clang-format off
  //dc.generatePath({
  //                 Point{0_in, 0_in, 0_deg},
  //                 Point{-1_in, 0_in, 0_deg},
  //                 Point{38_in, 0_in, 0_deg},
  //  }, "A");
  //dc.generatePath({
  //                 Point{0_in, 0_in, 0_deg},
  //                 Point{24_in, 0_in, 0_deg},
  //                 Point{34_in, 12_in, -90_deg},
  //  }, "B");
  //// clang-format on
}

void autonDriveStraight() {
  intake.move(127);

  drive.setMaxVelocity(115);
  drive.moveDistance(-39_in);

  intake.moveRelative(1260, 200);
  intake.moveRelative(-300, 100);

  auto a = (left.getPosition() + right.getPosition()) / 2;
  left.moveAbsolute(a, 75);
  right.moveAbsolute(a, 75);

  drive.moveDistance(24_in);
}
