#include "autonomous.h"
#include "util.h"

void autonSkills() {
  autonRedFlags();

  // turn, drive forward, turn again, then get on the platform
  drive::dc.turnAngle(95_deg);
  drive::dc.moveDistance(-41_in);
  drive::dc.turnAngle(-90_deg);
  drive::left.moveVelocity(-175);
  drive::right.moveVelocity(-175);
  delay(4250);
  drive::dc.stop();
}
