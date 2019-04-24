#include "autonomous.h"
#include "util.h"

#define side_Red 1
#define side_Blue -1

void autonCap(int side) {
  // set the intake to on
  intake.move(127);

  // drive forward, pick up the ball, and back up
  drive::dpc->setTarget("F1");
  drive::dpc->waitUntilSettled();
  drive::dpc->setTarget("10", true);
  drive::dpc->waitUntilSettled();

  // move away from the platforms to allow room to turn
  drive::strafe(500, 110);
  drive::waitUntilCompletion();

  // turn
  drive::turn(90 * side);


  // stop the intake
  intake.move(0);

  // align along the x
  vision::back.alignX(vision::Alignment::Platform, 200, 0, 5);
  delay(200);

  // align along the y
  vision::back.alignY(vision::Alignment::Platform, 35);

  // get onto the platform
  drive::reset();
  for (float i = 0; i < 1.f; i += .05) { // ramp up speed
    drive::control(-i, 0, 0);
    delay(25);
  }

  drive::control(-1, 0, 0);

  // stop when the robot is up
  waitUntil(drive::left.getPosition() < -3.25 * imev5GreenTPR, 20);
  waitUntil(drive::right.getPosition() < -3.25 * imev5GreenTPR, 20);
  drive::left.setBrakeMode(AbstractMotor::brakeMode::hold);
  drive::right.setBrakeMode(AbstractMotor::brakeMode::hold);
  drive::moveVelocity(0, 0);
}

void autonRedCaps() { autonCap(side_Red); }

void autonBlueCaps() { autonCap(side_Blue); }
