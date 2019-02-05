#include "autonomous.h"
#include "util.h"

#define side_red -1
#define side_blue 1

void autonFlags(int side) {
  // set the intake to on
  intake.move(127);

  // drive forward and grab the ball from under the cap
  drive::dc.setMaxVelocity(115);
  drive::dc.moveDistance(-38_in);

  // move the balls in the intake away from the flywheel
  intake.moveRelative(-180, 100);

  // back up
  drive::dc.moveDistance(33_in);

  // turn to the flags
  drive::dc.turnAngleAsync(90_deg * side);

  // once the intake stops moving, power up the flywheel
  waitUntil(motorPosTargetReached(intake, 20), 20);
  launcher.moveVelocity(fpreset[MAGIC]);

  // move towards the flags
  drive::dc.waitUntilSettled();
  drive::dc.moveDistanceAsync(-8_in);

  // wait for the drive and launcher to reach their position and velocity targets, respectively
  drive::dc.waitUntilSettled();
  waitUntil(motorVelTargetReached(launcher, 10), 20);

  // launch a ball
  intake.moveRelative(230, 200);
  waitUntil(motorPosTargetReached(intake, 20), 20);

  // drive forward and launch the second ball
  drive::dc.moveDistance(-16_in);
  delay(250);
  waitUntil(motorVelTargetReached(launcher, 10), 20);
  intake.moveRelative(520, 200);
  waitUntil(motorPosTargetReached(intake, 20), 20);
  intake.moveRelative(180, 200);

  // turn
  drive::dc.setMaxVelocity(200);
  drive::dc.turnAngle(20_deg * side);

  // make sure the second ball is launched
  waitUntil(motorPosTargetReached(intake, 20), 20);

  // stop launcher & intake
  launcher.moveVelocity(0);
  intake.moveVelocity(0);

  // back into the flag
  drive::dc.moveDistance(-16_in);
}

void flipCap(int side) {
  // turn, back away from the wall and turn towards the cap
  drive::dc.turnAngle(-20_deg * side);
  drive::dc.moveDistance(17_in);
  drive::dc.turnAngle(-95_deg * side);

  // flip the cap
  drive::dc.setMaxVelocity(150);
  intake.move(-127);
  drive::dc.moveDistance(-17_in);

  // back up from the cap and stop the intake
  drive::dc.moveDistance(14_in);
  intake.move(0);
}

void autonRedFlags() {
  autonFlags(side_red);
  flipCap(side_red);
}

void autonBlueFlags() {
  autonFlags(side_blue);
  flipCap(side_blue);
}
