#include "autonomous.h"
#include "util.h"

#define side_red -1
#define side_blue 1

void autonFlags(int side) {
  intake.move(127);

  drive::dc.setMaxVelocity(115);
  drive::dc.moveDistance(-39_in);

  intake.moveRelative(1260, 200);
  intake.moveRelative(-300, 100);

  auto a = (drive::left.getPosition() + drive::right.getPosition()) / 2;
  drive::left.moveAbsolute(a, 75);
  drive::right.moveAbsolute(a, 75);

  drive::dc.moveDistance(42_in);
  drive::dc.turnAngle(93_deg * side);
  drive::dc.moveDistance(-19.5_in);
  launcher.moveVelocity(fpreset[HIGH]);
  delay(2100);
  intake.moveRelative(540, 200);
  delay(2100);
  intake.move(127);
  drive::dc.moveDistance(-16_in);

  // stop launcher and intake
  launcher.moveVelocity(0);
  intake.move(0);

  drive::dc.setMaxVelocity(200);
  drive::dc.turnAngle(20_deg * side);
  drive::dc.moveDistance(-20_in);
}

void autonRedFlags() { autonFlags(side_red); }

void autonBlueFlags() { autonFlags(side_blue); }
