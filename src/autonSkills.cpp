#include "autonomous.h"
#include "util.h"

long int glt;

void autonSkills1() {
  glt = millis();

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
  auto ballGrabber = 8_rmtr;
  ballGrabber.setBrakeMode(AbstractMotor::brakeMode::hold);

  // intake in
  intake.moveVelocity(200);

  // grab the ball from under the cap
  drive::dpc.setTarget("F1");
  catapult::ready(); // ready catapult
  drive::dpc.waitUntilSettled();

  // allow the ball to enter the catapult
  delay(1200);

  // reverse intake and flip the cap
  drive::dpc.setTarget("48");
  delay(350);
  intake.moveVelocity(-200);
  drive::dpc.waitUntilSettled();

  // back up, intake in to make sure the ball is in the catapult
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

  drive::dpc.setTarget("48");
  drive::dpc.waitUntilSettled();

  // back up
  drive::dpc.setTarget("36", true);

  delay(500);

  // intake stop
  intake.moveVelocity(0);

  drive::dpc.waitUntilSettled();

  // align on the platforms
  drive::strafe(-1200, 125);
  drive::waitUntilCompletion();
  drive::reset();

  // back up and turn towards the flags
  drive::dpc.setTarget("F2", true);
  drive::dpc.waitUntilSettled();

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
  drive::strafe(-133, 110);
  drive::waitUntilCompletion();
  delay(200);

  // hit the bottom flag
  drive::dpc.setTarget("30");
  intake.moveVelocity(200);
  drive::dpc.waitUntilSettled();
  delay(100);
  drive::reset();

  // back up
  drive::dc.setMaxVelocity(180);

  // back up from the wall
  drive::dpc.setTarget("84", true);
  intake.moveVelocity(-200);
  drive::dpc.waitUntilSettled();
  intake.moveVelocity(0);

  // align against the wall
  drive::strafe(-650, 110);
  drive::waitUntilCompletion();
  drive::strafe(300, 110);
  drive::waitUntilCompletion();

  // turn towards the platform
  drive::moveRelative(560, -560, 120);
  drive::waitUntilCompletion();

  // ready catapult
  catapult::ready();

  // drive towards the platform
  drive::dpc.setTarget("15");
  drive::dpc.waitUntilSettled();

  // grab the ball and back up
  intake.moveVelocity(200);
  ballGrabber.moveAbsolute(300, 200);
  waitUntil(ballGrabber.getActualVelocity() > 5, 20);
  waitUntil(ballGrabber.getActualVelocity() < 5, 20);
  drive::dpc.setTarget("10", true);
  drive::dpc.waitUntilSettled();

  // // intake the ball and retract the grabber
  // delay(500);
  // ballGrabber.moveAbsolute(0, 200);

  // turn to an angle
  drive::moveRelative(70, -70, 120);
  drive::waitUntilCompletion();

  // strafe over to the next cap
  drive::reset();
  drive::strafe(-1160, 120);
  drive::waitUntilCompletion();

  // intake in
  intake.moveVelocity(200);

  // grab the ball from under the cap
  ballGrabber.moveAbsolute(0, 200);
  drive::dpc.setTarget("48");
  drive::dpc.waitUntilSettled();

  // back up a bit
  drive::dpc.setTarget("2", true);
  drive::dpc.waitUntilSettled();

  // allow the ball to enter the catapult
  delay(1050);

  // reverse intake and flip the cap
  drive::dpc.setTarget("40");
  delay(200);
  intake.moveVelocity(-200);
  drive::dpc.waitUntilSettled();

  // back up, intake in to make sure the ball is in the catapult
  drive::dpc.setTarget("30", true);
  intake.moveVelocity(200);
  drive::dpc.waitUntilSettled();

  // move away from the platforms to allow room to turn
  drive::strafe(-230, 110);
  drive::waitUntilCompletion();

  // turn towards the flags and align
  drive::moveRelative(-540, 540, 120);
  delay(200);
  intake.moveVelocity(200);
  drive::waitUntilCompletion();
  drive::dpc.setTarget("2");
  drive::dpc.waitUntilSettled();
  drive::strafe(-80, 110);
  drive::waitUntilCompletion();

  // fire the catapult
  catapult::fire();
  intake.moveVelocity(200);
  waitUntil(catapult::pot.get() > catapult::presets[3], 20);
  delay(1000);

  // align to the bottom flag
  drive::reset();
  drive::strafe(-200, 110);
  drive::waitUntilCompletion();

  // hit the bottom flag
  drive::dpc.setTarget("42");
  drive::dpc.waitUntilSettled();
  delay(100);
  drive::reset();

  // back up from the wall
  drive::dpc.setTarget("26", true);
  intake.moveVelocity(-200);
  drive::dpc.waitUntilSettled();

  // turn towards the cap
  drive::moveRelative(-550, 550, 120);
  drive::waitUntilCompletion();

  // flip the cap
  drive::dpc.setTarget("30");
  drive::dpc.waitUntilSettled();

  // back up
  drive::dpc.setTarget("6", true);
  drive::dpc.waitUntilSettled();

  // stop the intake
  intake.moveVelocity(0);

  // strafe to line up to drive upon the platform
  drive::strafe(-750, 120);
  drive::waitUntilCompletion();
  drive::dpc.setTarget("18");
  drive::dpc.waitUntilSettled();
  drive::strafe(-880, 120);
  drive::waitUntilCompletion();

  // align
  drive::moveRelative(60, -60, 120);
  drive::waitUntilCompletion();

  // align on the wall
  // drive::moveVelocity(90, 90);
  // drive::waitUntilStarted();
  // delay(1600);

  drive::dpc.setTarget("12");
  drive::dpc.waitUntilSettled();

  // get onto the platform
  drive::dc.setMaxVelocity(200);
  drive::left.moveRelative(-2300, 200);
  drive::right.moveRelative(-2300, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);
  drive::moveVelocity(0, 0);

  glt = millis() - glt;
}
