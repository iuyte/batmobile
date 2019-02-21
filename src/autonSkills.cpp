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
  // intake in
  intake.moveVelocity(200);

  // grab the ball from under the cap
  drive::dpc.setTarget("F1");
  catapult::ready(); // ready catapult
  drive::dpc.waitUntilSettled();

  // allow the ball to enter the catapult
  delay(1500);

  // reverse intake and flip the cap
  intake.moveVelocity(-200);
  // drive::dc.setMaxVelocity(90);
  // drive::dc.moveDistance(30_in);
  // delay(250);

  drive::dpc.setTarget("48");
  drive::dpc.waitUntilSettled();

  // back up, intake in to make sure the ball is in the catapult
  // drive::dc.setMaxVelocity(180);
  intake.moveVelocity(200);

  drive::dpc.setTarget("60", true);
  drive::dpc.waitUntilSettled();

  // wait a bit
  drive::dc.stop();
  delay(250);

  // move sideways to line up to the right cap
  drive::reset();
  drive::strafe(720, 125);
  drive::waitUntilCompletion();
  drive::dc.stop();
  delay(250);

  // reverse intake and flip the cap
  intake.moveVelocity(-200);
  // drive::dc.setMaxVelocity(90);
  // drive::dc.moveDistance(30_in);
  // drive::dc.setMaxVelocity(200);
  // delay(250);

  drive::dpc.setTarget("48");
  drive::dpc.waitUntilSettled();

  // back up
  drive::dpc.setTarget("36", true);

  delay(500);

  // intake in slowly
  intake.moveVelocity(0);

  drive::dpc.waitUntilSettled();

  // align on the platforms
  drive::strafe(-1100, 125);
  drive::waitUntilCompletion();
  drive::reset();

  // back up and turn towards the flags
  drive::dpc.setTarget("F2", true);
  drive::dpc.waitUntilSettled();
  // drive::turn(-89);

  drive::moveRelative(-555, 555, 120);
  drive::waitUntilCompletion();

  // drive towards the flags
  drive::dpc.setTarget("60");
  drive::dpc.waitUntilSettled();

  // wait a bit
  drive::moveVelocity(0, 0);
  waitUntil(catapult::atTarget(), 20);
  delay(250);

  // fire the catapult
  catapult::fire();
  waitUntil(catapult::pot.get() > catapult::presets[3], 20);
  delay(1000);

  // align to the bottom flag
  drive::reset();
  drive::strafe(-160, 110);
  drive::waitUntilCompletion();
  delay(200);

  // hit the bottom flag
  drive::dpc.setTarget("42");
  drive::dpc.waitUntilSettled();
  delay(100);
  drive::reset();

  // drive::moveVelocity(125, 125);
  // drive::waitUntilStarted();
  // drive::waitUntilStopped();
  // drive::moveVelocity(0, 0);

  // back up
  drive::dc.setMaxVelocity(180);

  // back up from the wall
  drive::dpc.setTarget("84", true);
  drive::dpc.waitUntilSettled();

  // turn towards the platform
  drive::moveRelative(555, -555, 120);
  drive::waitUntilCompletion();
}
