#include "autonomous.h"
#include "util.h"

void autonSkills() {
  autonRedFlags();

  drive::dc.setMaxVelocity(125);
  // turn, reverse intake and move towards tha cap a bit
  drive::dc.turnAngle(45_deg);
  intake.move(-127);
  drive::dc.moveDistance(-30_in);

  // turn towards tha cap and flip it
  drive::dc.turnAngle(-40_deg);

  drive::dc.setMaxVelocity(200);
  drive::dc.moveDistance(-24_in);
  drive::dc.setMaxVelocity(125);

  // back up and reverse intake
  drive::dc.moveDistance(48_in);
  intake.moveVelocity(100);

  // turn, drive forward, turn again, then back up
  drive::dc.turnAngle(90_deg);
  drive::dc.moveDistance(-22_in);
  drive::dc.turnAngle(-90_deg);
  drive::dc.moveDistance(7_in);

  // get onto the platform
  drive::left.moveVelocity(-175);
  drive::right.moveVelocity(-175);
  delay(3250);
  drive::moveVelocity(0, 0);
}
