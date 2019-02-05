#include "autonomous.h"
#include "util.h"

void nautonSkills() {
  autonRedFlags();

  drive::dc.setMaxVelocity(125);
  // turn, reverse intake and move towards tha cap a bit
  drive::dc.turnAngle(35_deg);
  intake.move(-200);
  drive::dc.moveDistance(-35_in);

  // turn towards tha cap and flip it
  drive::dc.turnAngle(-30_deg);

  drive::dc.setMaxVelocity(200);
  drive::dc.moveDistance(-19_in);
  drive::dc.setMaxVelocity(125);

  // back up and stop intake
  drive::dc.moveDistance(42_in);
  intake.moveVelocity(0);

  // turn, drive forward, turn again
  drive::dc.turnAngle(85_deg);
  drive::dc.moveDistance(-43.5_in);
  drive::dc.turnAngle(-90_deg);

  // flip the next cap then back up
  intake.moveVelocity(-200);
  drive::dc.moveDistance(-36_in);
  drive::dc.moveDistance(36_in);

  // turn, go forward, turn again
  drive::dc.turnAngle(90_deg);
  drive::dc.moveDistance(-17.5_in);
  drive::dc.turnAngle(-85_deg);

  // flip yet another cap and back up
  drive::dc.moveDistance(-40_in);
  drive::dc.moveDistance(42_in);

  // reverse intake, turn and drive towards the platform
  intake.moveVelocity(100);
  drive::dc.turnAngle(-95_deg);
  drive::dc.moveDistance(-44_in);

  // align to get on the platform
  drive::dc.turnAngle(93_deg);
  drive::dc.moveDistance(7_in);

  // get onto the platform
  drive::dc.setMaxVelocity(200);
  // drive::left.moveVelocity(-175);
  // drive::right.moveVelocity(-175);
  // delay(3250);
  drive::left.moveRelative(-1800, 200);
  drive::right.moveRelative(-1800, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);
  drive::moveVelocity(0, 0);
}

void autonSkills() {
  autonRedFlags();

  drive::dc.setMaxVelocity(125);
  // turn, reverse intake and move towards tha cap a bit
  drive::dc.turnAngle(30_deg);
  intake.move(-85);
  drive::dc.moveDistance(-34.5_in);

  // turn towards tha cap and flip it
  drive::dc.turnAngle(-25_deg);

  drive::dc.setMaxVelocity(200);
  drive::dc.moveDistance(-18_in);
  drive::dc.setMaxVelocity(125);

  // back up and reverse intake
  drive::dc.moveDistance(48_in);
  intake.moveVelocity(100);

  // turn, drive forward, turn again, then back up
  drive::dc.turnAngle(78_deg);
  drive::dc.moveDistance(-22_in);
  drive::dc.turnAngle(-90_deg);
  drive::dc.moveDistance(7_in);

  // get onto the platform
  drive::dc.setMaxVelocity(200);
  drive::left.moveRelative(-1800, 200);
  drive::right.moveRelative(-1800, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);
  drive::moveVelocity(0, 0);
}
