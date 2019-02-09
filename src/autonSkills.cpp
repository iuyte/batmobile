#include "autonomous.h"
#include "util.h"

void autonSkills() {
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
  drive::left.moveRelative(-1800, 200);
  drive::right.moveRelative(-1800, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);
  drive::moveVelocity(0, 0);
}
