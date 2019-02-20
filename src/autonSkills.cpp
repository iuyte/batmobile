#include "autonomous.h"
#include "util.h"

void autonSkills1() {
  autonRedFlagsCap();

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
  // move sideways to line up to the right cap
  drive::strafe(800, 100);
  delay(250);
  drive::waitUntilCompletion();

  drive::turn(0, 2, true);

  // reverse intake and flip the cap
  intake.moveVelocity(-60);
  drive::dpc.setTarget("F1");
  drive::dpc.waitUntilSettled();
  drive::dc.setMaxVelocity(90);
  drive::dc.moveDistance(12_in);
  drive::dc.setMaxVelocity(200);
  delay(500);

  // back up
  drive::dc.moveDistance(-24_in);

  // intake in and ready the catapult
  intake.moveVelocity(200);
  catapult::ready();

  exit(0);

  // align to the left cap
  drive::strafe(-800, 100);
  drive::waitUntilCompletion();
  drive::turn(0, 2, true);

  // grab the ball into the catapult
  drive::dc.moveDistance(12_in);
  delay(1500);

  // flip the cap
  intake.moveVelocity(-60);
  drive::dc.setMaxVelocity(90);
  drive::dc.moveDistance(6_in);
  drive::dc.moveDistance(-6_in);
  drive::dc.setMaxVelocity(200);

  // intake in again
  intake.moveVelocity(200);

  // back up and turn towards the flags
  drive::dpc.setTarget("F2", true);
  drive::dpc.waitUntilSettled();
  drive::turn(-89, true);

  // drive towards the flags
  drive::dc.moveDistance(60_in);

  // wait a bit
  drive::moveVelocity(0, 0);
  waitUntil(catapult::atTarget(), 20);
  delay(250);

  // fire the catapult
  catapult::fire();
  waitUntil(catapult::pot.get() > catapult::presets[3], 20);
  delay(800);

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

  // back up
  drive::dc.setMaxVelocity(180);
  drive::dc.moveDistance(-48_in);
}
