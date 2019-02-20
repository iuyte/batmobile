#include "autonomous.h"
#include "util.h"

#define side_Red 1
#define side_Blue -1

void autonFlags(int side) {
  // set the intake to on
  intake.move(127);

  // drive forward and grab the ball
  drive::dpc.setTarget("F1");
  drive::dpc.waitUntilSettled();

  // stop - not necessary but seems to help
  drive::moveVelocity(0, 0);

  // back up
  drive::dpc.setTarget("F2", true);

  // ready the catapult
  catapult::ready();

  // wait for backing up to finish
  drive::dpc.waitUntilSettled();

  // turn towards the flags
  drive::turn(-90 * side);

  // drive forward a bit
  drive::dpc.setTarget("F3");
  drive::dpc.waitUntilSettled();

  waitUntil(catapult::atTarget(), 20);

  // wait a bit
  drive::moveVelocity(0, 0);
  delay(250);

  // fire the catapult
  catapult::fire();
  waitUntil(catapult::pot.get() > catapult::presets[3], 20);
  delay(800);

  // don't
  // reverse intake for cap flipping
  // intake.move(-127);

  // align to the bottom flag
  drive::reset();
  drive::strafe(-160, 110);
  waitUntil(drive::totalVelocity() > 4, 20);
  waitUntil(drive::totalVelocity() < 4, 20);
  delay(200);
  // delay(400);
  drive::turn(0, 2, true);

  // hit the bottom flag
  drive::dc.setMaxVelocity(125);
  drive::moveVelocity(125, 125);
  waitUntil(drive::totalVelocity() > 10, 20);
  waitUntil(drive::totalVelocity() < 10, 20);
  drive::moveVelocity(0, 0);
  // drive::dc.moveDistance(42_in);
  // drive::dc.moveDistance(16_in);
  // if (abs(drive::getAngle()) > 10)
  // drive::turn(0, 2, true);
  // drive::dc.moveDistance(14_in);
  // drive::dc.moveDistance(12_in);
  // drive::dc.tank(.2, .2);
  // delay(100);
  // drive::reset();

  // back up
  drive::dc.moveDistance(-10_in);
  drive::dc.setMaxVelocity(200);
}

void flipCap(int side) {
  // back up from the wall
  drive::dpc.setTarget("FC1", true);
  drive::dpc.waitUntilSettled();

  // turn towards the cap
  drive::turn(90 * side);

  // flip the cap
  intake.moveVelocity(-60);
  drive::dc.setMaxVelocity(75);
  drive::dc.moveDistance(24_in);

  // back away
  drive::dpc.setTarget("F3", true);
  drive::dpc.waitUntilSettled();

  // stop the intake
  intake.move(0);
  drive::dc.setMaxVelocity(200);
}

void scoreTree2(int side) {
  // back up from the wall
  drive::dpc.setTarget("FC1", true);

  // ready the catapult
  catapult::ready();

  // wait to achieve the position
  drive::dpc.waitUntilSettled();

  // strafe towards the cap
  drive::strafe(1300);
  waitUntil(drive::totalVelocity() > 4, 20);
  waitUntil(drive::totalVelocity() < 4, 20);
  drive::dc.stop();

  // turn, align
  drive::turn(45 * side);
  drive::dc.moveDistance(4_in);
  // drive::strafe(-180, 90);

  // fire
  delay(150);
  catapult::fire();
  delay(250);

  // back away
  drive::dc.moveDistance(-4_in);
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
