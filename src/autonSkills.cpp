#include "autonomous.h"
#include "util.h"

void autonSkills() {
  // set the intake to on
  intake.move(127);

  // drive forward and grab the ball from under the cap
  drive::dc.setMaxVelocity(115);
  drive::dc.moveDistance(-39_in);

  // make sure the ball is intaked, then move the balls in the intake away from the flywheel
  intake.moveRelative(30, 200);
  waitUntil(motorPosTargetReached(intake, 10), 20);
  intake.moveRelative(-180, 100);
  waitUntil(motorPosTargetReached(intake, 10), 20);

  // back up and turn to the flags
  drive::dc.moveDistance(41_in);
  drive::dc.turnAngle(-93.5_deg);

  // drive towards the flags and power up the flywheel
  drive::dc.moveDistanceAsync(-26_in);
  launcher.moveVelocity(fpreset[HIGH]);

  // wait for the drive and launcher to reach their position and velocity targets, respectively
  drive::dc.waitUntilSettled();
  waitUntil(motorVelTargetReached(launcher, 10), 20);

  // launch a ball
  intake.moveRelative(210, 200);
  waitUntil(motorPosTargetReached(intake, 10), 20);

  // drive forward and launch the second ball
  drive::dc.moveDistance(-13.5_in);
  delay(250);
  waitUntil(motorVelTargetReached(launcher, 10), 20);
  intake.moveRelative(480, 200);
  waitUntil(motorPosTargetReached(intake, 10), 20);

  // stop launcher
  launcher.moveVelocity(0);
  intake.moveVelocity(100);

  // turn and hit the bottom flag
  drive::dc.turnAngle(-65_deg);
  drive::dc.moveDistance(-6_in);
  drive::dc.turnAngle(62.5_deg);
  drive::dc.moveDistance(-16_in);

  // align on the wall
  drive::dc.tank(-.375, -.375);
  delay(250);
  waitUntil(drive::left.getEfficiency() + drive::right.getEfficiency() < 10, 20);
  drive::dc.stop();

  intake.moveVelocity(0);

  // turn, back away from the wall and turn towards the cap
  drive::dc.moveDistance(4_in);
  drive::dc.turnAngle(-15_deg);
  drive::dc.moveDistance(26_in);
  drive::dc.turnAngle(113_deg);

  // flip the cap
  intake.move(-127);
  drive::dc.moveDistance(-22_in);

  // back up from the cap
  drive::dc.moveDistance(18_in);

  // turn, drive forward, turn again, then get on the platform
  drive::dc.turnAngle(95_deg);
  drive::dc.moveDistance(-43.5_in);
  drive::dc.turnAngle(-95_deg);
  drive::left.moveVelocity(-175);
  drive::right.moveVelocity(-175);
  delay(6000);
  drive::dc.stop();
}
