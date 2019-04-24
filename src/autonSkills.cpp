#include "autonomous.h"
#include "util.h"

long int glt;

void autonSkills1() {
  glt = millis();

  // intake in
  intake.moveVelocity(200);

  // grab the ball from under the cap
  drive::dpc->setTarget("F1");
  catapult::ready(); // ready catapult
  drive::dpc->waitUntilSettled();

  // back up a bit
  drive::dpc->setTarget("2", true);
  drive::dpc->waitUntilSettled();

  // allow the ball to enter the catapult
  delay(900);

  // reverse intake and flip the cap
  drive::dpc->setTarget("48");
  delay(350);
  intake.moveVelocity(-200);
  drive::dpc->waitUntilSettled();

  // back up, intake in to make sure the ball is in the catapult
  intake.moveVelocity(200);

  drive::dpc->setTarget("60", true);
  drive::dpc->waitUntilSettled();

  // wait a bit
  drive::dc->stop();
  delay(250);

  // move sideways to line up to the right cap
  drive::reset();
  drive::strafe(720, 125);
  drive::waitUntilCompletion();
  drive::dc->stop();
  delay(250);

  // reverse intake and flip the cap
  intake.moveVelocity(-200);

  drive::dpc->setTarget("48");
  drive::dpc->waitUntilSettled();

  // back up
  drive::dpc->setTarget("36", true);

  delay(500);

  // intake stop
  intake.moveVelocity(0);

  drive::dpc->waitUntilSettled();

  // align on the platforms
  drive::strafe(-1010, 125);
  drive::waitUntilCompletion();
  drive::reset();

  // back up and turn towards the flags
  drive::dpc->setTarget("F2", true);
  drive::dpc->waitUntilSettled();

  drive::moveRelative(-555, 555, 120);
  drive::waitUntilCompletion();

  // drive towards the flags
  drive::dpc->setTarget("60");
  drive::dpc->waitUntilSettled();

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
  drive::dpc->setTarget("30");
  intake.moveVelocity(200);
  drive::dpc->waitUntilSettled();
  delay(100);
  drive::reset();

  // back up
  drive::dc->setMaxVelocity(180);

  // back up from the wall
  drive::dpc->setTarget("84", true);
  intake.moveVelocity(-200);
  drive::dpc->waitUntilSettled();
  intake.moveVelocity(0);

  // align against the wall
  drive::strafe(-500, 110);
  drive::waitUntilCompletion();
  drive::strafe(300, 110);
  drive::waitUntilCompletion();

  // turn towards the platform
  drive::moveRelative(560, -560, 120);
  drive::waitUntilCompletion();

  // ready catapult
  catapult::ready();

  // drive towards the platform
  drive::dpc->setTarget("15");
  drive::dpc->waitUntilSettled();

  // grab the ball and back up
  intake.moveVelocity(200);
  arm.moveAbsolute(300, 200);
  waitUntil(arm.getActualVelocity() > 5, 20);
  waitUntil(arm.getActualVelocity() < 5, 20);
  drive::dpc->setTarget("10", true);
  drive::dpc->waitUntilSettled();

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
  arm.moveAbsolute(0, 200);
  drive::dpc->setTarget("48");
  drive::dpc->waitUntilSettled();

  // back up a bit
  drive::dpc->setTarget("4", true);
  drive::dpc->waitUntilSettled();

  // allow the ball to enter the catapult
  delay(1050);

  // reverse intake and flip the cap
  drive::dpc->setTarget("44");
  delay(200);
  intake.moveVelocity(-200);
  drive::dpc->waitUntilSettled();

  // back up, intake in to make sure the ball is in the catapult
  drive::dpc->setTarget("36", true);
  intake.moveVelocity(200);
  drive::dpc->waitUntilSettled();

  // move away from the platforms to allow room to turn
  drive::strafe(-230, 110);
  drive::waitUntilCompletion();

  // turn towards the flags and align
  drive::moveRelative(-549, 549, 120);
  delay(200);
  intake.moveVelocity(200);
  drive::waitUntilCompletion();
  drive::dpc->setTarget("2");
  drive::dpc->waitUntilSettled();
  // drive::strafe(-80, 110);
  // drive::waitUntilCompletion();

  // fire the catapult
  catapult::fire();
  intake.moveVelocity(200);
  waitUntil(catapult::pot.get() > catapult::presets[3], 20);
  delay(1000);

  // align to the bottom flag
  drive::reset();
  drive::strafe(-200, 110);
  drive::waitUntilCompletion(200);

  // hit the bottom flag
  drive::dpc->setTarget("42");
  drive::dpc->waitUntilSettled();
  delay(100);
  drive::reset();

  // back up from the wall
  drive::dpc->setTarget("26", true);
  intake.moveVelocity(-200);
  drive::dpc->waitUntilSettled();

  // turn towards the cap
  drive::moveRelative(-550, 550, 120);
  drive::waitUntilCompletion();

  // flip the cap
  drive::dpc->setTarget("36");
  drive::dpc->waitUntilSettled();

  // back up
  drive::dpc->setTarget("6", true);
  drive::dpc->waitUntilSettled();

  // stop the intake
  intake.moveVelocity(0);

  // strafe to line up to drive upon the platform
  drive::strafe(-750, 120);
  drive::waitUntilCompletion();
  drive::dpc->setTarget("18");
  drive::dpc->waitUntilSettled();
  drive::moveRelative(75, -75, 120);
  drive::waitUntilCompletion();
  drive::strafe(-700, 120);
  drive::waitUntilCompletion();

  // align along the x
  vision::back.alignX(vision::Alignment::Platform, 200, 0, 5);
  delay(200);

  // align along the y
  vision::back.alignY(vision::Alignment::Platform, 26);

  // get onto the platform
  drive::reset();
  for (float i = 0; i < 1.f; i += .05) {
    drive::control(-i, 0, 0);
    delay(25);
  }

  drive::control(-1, 0, 0);

  waitUntil(drive::left.getPosition() < -6 * imev5GreenTPR, 20);
  waitUntil(drive::right.getPosition() < -6 * imev5GreenTPR, 20);

  drive::left.setBrakeMode(AbstractMotor::brakeMode::hold);
  drive::right.setBrakeMode(AbstractMotor::brakeMode::hold);
  drive::moveVelocity(0, 0);

  glt = millis() - glt;
}

void autonSkills2() {
  glt = millis();

  // intake in, arm down, ready catapult
  intake.moveVelocity(200);
  arm.moveAbsolute(ArmP::Tipped, 150);
  catapult::ready();

  // grab the ball from under the cap
  drive::dpc->setTarget("36");
  drive::dpc->waitUntilSettled();

  // flip the cap
  drive::dpc->setTarget("2");
  arm.moveAbsolute(ArmP::Top, 200);
  drive::dpc->waitUntilSettled();

  // back up
  drive::dpc->setTarget("34", true);
  drive::dpc->waitUntilSettled();

  // turn and drive towards the flags
  drive::turn(-90);
  delay(200);
  drive::dpc->setTarget("60");
  drive::dpc->waitUntilSettled();

  // aling to shoot
  vision::front.alignX(vision::Alignment::Flag, 75, 14, 2);
  drive::waitUntilCompletion();

  // drive forward a bit
  // drive::dpc->setTarget("12");
  // drive::dpc->waitUntilSettled();

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
  vision::front.alignX(vision::Alignment::Flag, 75, -20, 2);
  delay(200);

  // hit the bottom flag
  intake.move(127);
  drive::dpc->setTarget("28");
  delay(750);
  arm.moveAbsolute(ArmP::Flag, 200);
  drive::dpc->waitUntilSettled();

  // back up
  intake.move(-127);
  drive::dpc->setTarget("52", true);
  drive::dpc->waitUntilSettled();
  intake.move(0);

  // turn towards the platform
  drive::turn(90);
  delay(100);

  intake.moveVelocity(200);
  arm.moveAbsolute(ArmP::Mid, 100);
  drive::dpc->setTarget("10");
  drive::dpc->waitUntilSettled();
  arm.moveAbsolute(ArmP::Low, 150);
  delay(400);
  drive::dpc->setTarget("8", true);
  drive::dpc->waitUntilSettled();
  arm.moveAbsolute(ArmP::Top, 200);

  drive::strafe(-330, 130);
  drive::waitUntilCompletion(200);

  // intake in, arm down, ready catapult
  intake.moveVelocity(200);
  arm.moveAbsolute(ArmP::Tipped, 150);
  catapult::ready();

  // grab the ball from under the cap
  drive::dpc->setTarget("30");
  drive::dpc->waitUntilSettled();

  // flip the cap
  drive::dpc->setTarget("2");
  arm.moveAbsolute(ArmP::Top, 200);
  drive::dpc->waitUntilSettled();

  // turn and drive towards the flags
  drive::turn(-90);
  delay(200);
  drive::dpc->setTarget("16");
  drive::dpc->waitUntilSettled();

  // aling to shoot
  vision::front.alignX(vision::Alignment::Flag, 75, 14, 2);
  drive::waitUntilCompletion();

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
  vision::front.alignX(vision::Alignment::Flag, 75, -20, 2);
  delay(200);

  // hit the bottom flag
  intake.move(127);
  drive::dpc->setTarget("28");
  delay(750);
  arm.moveAbsolute(ArmP::Flag, 200);
  drive::dpc->waitUntilSettled();

  // back up
  intake.move(-127);
  drive::dpc->setTarget("20", true);
  drive::dpc->waitUntilSettled();
  intake.move(0);

  return;
  // turn towards the cap
  drive::turn(45);
  intake.move(127);
  delay(100);

  arm.moveAbsolute(ArmP::Mid, 100);
  drive::dpc->setTarget("16");
  drive::dpc->waitUntilSettled();

  arm.moveAbsolute(ArmP::Tipped, 100);
  delay(200);

  drive::dpc->setTarget("6", true);
  drive::dpc->waitUntilSettled();
  drive::dpc->setTarget("8");
  drive::dpc->waitUntilSettled();

  glt = millis() - glt;
}
