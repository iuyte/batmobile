#include "autonomous.h"
#include "util.h"

void autonSkills() {
  intake.move(127);

  drive.setMaxVelocity(115);
  drive.moveDistance(-39_in);

  intake.moveRelative(1260, 200);
  intake.moveRelative(-300, 100);

  auto a = (left.getPosition() + right.getPosition()) / 2;
  left.moveAbsolute(a, 75);
  right.moveAbsolute(a, 75);

  drive.moveDistance(42_in);
  drive.turnAngle(-92_deg);
  drive.moveDistance(-20_in);
  launcher.moveVelocity(fpreset[HIGH]);
  delay(2100);
  intake.moveRelative(540, 200);
  delay(2100);
  launcher.moveVelocity(fpreset[LOW]);
  drive.moveDistance(-16_in);
  delay(1000);
  intake.move(127);

  delay(1250);
  // stop launcher and intake
  launcher.moveVelocity(0);
  intake.move(0);

  drive.turnAngle(-20_deg);
  drive.moveDistance(-20_in);
}

#define side_red  -1
#define side_blue 1

void autonFlags(int side) {
  intake.move(127);

  drive.setMaxVelocity(115);
  drive.moveDistance(-39_in);

  intake.moveRelative(1260, 200);
  intake.moveRelative(-300, 100);

  auto a = (left.getPosition() + right.getPosition()) / 2;
  left.moveAbsolute(a, 75);
  right.moveAbsolute(a, 75);

  drive.moveDistance(42_in);
  drive.turnAngle(93_deg * side);
  drive.moveDistance(-19.5_in);
  launcher.moveVelocity(fpreset[HIGH]);
  delay(2100);
  intake.moveRelative(540, 200);
  delay(2100);
  intake.move(127);
  drive.moveDistance(-16_in);

  // stop launcher and intake
  launcher.moveVelocity(0);
  intake.move(0);

  drive.setMaxVelocity(200);
  drive.turnAngle(20_deg * side);
  drive.moveDistance(-20_in);
}

void autonRedFlags() {
  autonFlags(side_red);
}

void autonBlueFlags() {
  autonFlags(side_blue);
}
