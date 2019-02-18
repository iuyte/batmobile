#include "autonomous.h"
#include "util.h"

void autonSkills1() {
  autonRedFlags();

  // slower for better accuracy
  drive::dc.setMaxVelocity(110);

  // back up
  drive::dc.moveDistance(-18_in);

  // turn 90 deg counter-clockwise
  drive::dc.turnAngle(-96_deg);

  // drive towards the platform
  drive::dc.moveDistance(-50_in);

  // turn 90 deg counter-clockwise
  drive::dc.turnAngle(-90_deg);

  // get onto the platform
  drive::dc.setMaxVelocity(200);
  drive::left.moveRelative(-2400, 200);
  drive::right.moveRelative(-2400, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);
  drive::moveVelocity(0, 0);
}

void autonSkills2() {
  // set the intake to on
  intake.move(127);

  // drive forward and grab the ball
  drive::dpc.setTarget("F1");
  drive::dpc.waitUntilSettled();

  // ready the catapult
  catapult::ready();

  // wait for the ball to be in the catapult
  delay(2000);

  //

  // turn towards the flags
  drive::dc.setMaxVelocity(70);
  drive::dc.turnAngle(-93_deg);
  drive::dc.setMaxVelocity(200);

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
  delay(250);

  // reverse intake for cap flipping
  intake.move(-127);

  // hit the bottom flag
  drive::dc.setMaxVelocity(125);
  drive::dc.turnAngle(-24_deg);
  drive::dc.moveDistance(20_in);
  drive::dc.turnAngle(24_deg);
  drive::dc.moveDistance(22_in);
  drive::dc.moveDistance(-10_in);

  // back up from the wall
  drive::dpc.setTarget("FC1", true);
  drive::dpc.waitUntilSettled();

  // turn towards the cap
  drive::dc.setMaxVelocity(85);
  drive::dc.turnAngle(100_deg);

  // flip the cap
  drive::dc.moveDistance(24_in);

  // back away
  drive::dpc.setTarget("F3", true);
  drive::dpc.waitUntilSettled();

  // stop the intake
  intake.move(0);

  // slower for better accuracy
  drive::dc.setMaxVelocity(110);

  // back up
  drive::dc.moveDistance(-18_in);

  // turn 90 deg counter-clockwise
  drive::dc.turnAngle(-96_deg);

  // drive towards the platform
  drive::dc.moveDistance(-50_in);

  // turn 90 deg counter-clockwise
  drive::dc.turnAngle(-90_deg);

  // get onto the platform
  drive::dc.setMaxVelocity(200);
  drive::left.moveRelative(-2400, 200);
  drive::right.moveRelative(-2400, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);
  drive::moveVelocity(0, 0);
}
