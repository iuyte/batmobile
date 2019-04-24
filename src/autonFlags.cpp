#include "autonomous.h"
#include "util.h"

#define side_Red 1
#define side_Blue -1

void autonFlags(int side) {
  // set the intake to on
  intake.move(127);

  // drive forward and grab the ball
  drive::dpc->setTarget("F1");
  drive::dpc->waitUntilSettled();

  // stop - not necessary but seems to help
  drive::moveVelocity(0, 0);

  // back up
  intake.move(127);
  drive::dpc->setTarget("34", true);

  // ready the catapult
  catapult::ready();

  // wait for backing up to finish
  drive::dpc->waitUntilSettled();

  // turn towards the flags
  drive::turn(-90 * side);
  delay(200);

  // aling to shoot
  intake.move(127);
  vision::front.alignX(vision::Alignment::Flag, 75, 14 * side, 2);
  drive::waitUntilCompletion();

  // drive forward a bit
  drive::dpc->setTarget("F3");
  drive::dpc->waitUntilSettled();

  // wait a bit
  waitUntil(catapult::atTarget(), 20);
  drive::moveVelocity(0, 0);
  delay(250);

  // fire the catapult
  catapult::fire();
  waitUntil(catapult::pot.get() > catapult::presets[3], 20);
  delay(850);

  // align to the bottom flag
  drive::reset();
  vision::front.alignX(vision::Alignment::Flag, 75, -20 * side, 2);
  delay(200);

  // hit the bottom flag
  drive::dpc->setTarget("28");
  intake.move(127);
  delay(750);
  arm.moveAbsolute(ArmP::Flag, 200);
  drive::dpc->waitUntilSettled();

  // back up
  drive::dpc->setTarget("14", true);
  drive::dpc->waitUntilSettled();
  arm.moveAbsolute(ArmP::Top, 200);
}

void flipCap(int side) {
  // turn towards the cap
  drive::turn(96 * side);

  // flip the cap
  arm.moveAbsolute(ArmP::Low, 200);
  delay(300);
  drive::dpc->setTarget("6");
  drive::dpc->waitUntilSettled();
  drive::dpc->setTarget("4");
  arm.moveAbsolute(ArmP::Top, 200);
  drive::dpc->waitUntilSettled();

  // back away
  drive::dpc->setTarget("12", true);
  drive::dpc->waitUntilSettled();

  // stop the intake
  intake.move(0);
  drive::dc->setMaxVelocity(200);
}

void scoreTree2(int side) {
  intake.move(127);
  // ready the catapult
  catapult::ready();

  // wait to achieve the position
  drive::dpc->waitUntilSettled();

  // turn towards the flags
  drive::turn(65 * side);

  // drive towards the flags
  drive::dpc->setTarget("14");
  drive::dpc->waitUntilSettled();

  // fire
  catapult::fire();
  delay(1000);
}

void autonRedFlagsCap() {
  autonFlags(side_Red);
  flipCap(side_Red);
}

void autonBlueFlagsCap() {
  autonFlags(side_Blue);
  flipCap(side_Blue);
}

void autonRedFlagsX2() {
  autonFlags(side_Red);
  scoreTree2(side_Red);
}

void autonBlueFlagsX2() {
  autonFlags(side_Blue);
  scoreTree2(side_Blue);
}
