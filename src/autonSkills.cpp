#include "autonomous.h"
#include "util.h"

template <typename T> void logAndWait(T t) {
  static unsigned long i = 0;
  cout << t << ", " << i++ << endl;
  delay(250);
}

void autonSkills() {
  // set the intake to on
  intake.move(127);

  // drive forward and grab the ball from under the cap
  drive::dc.setMaxVelocity(115);
  drive::dc.moveDistance(-39_in);

  // make sure the ball is intaked, then move the balls in the intake away from the flywheel
  intake.moveRelative(1260, 200);
  intake.moveRelative(-180, 100);

  // back up and turn to the flags
  drive::dc.moveDistance(41.5_in);
  drive::dc.turnAngle(-92_deg);

  // drive towards the flags and power up the flywheel
  drive::dc.moveDistanceAsync(-25_in);
  launcher.moveVelocity(fpreset[HIGH]);

  // wait for the drive and launcher to reach their position and velocity targets, respectively
  drive::dc.waitUntilSettled();
  waitUntil(abs(launcher.getActualVelocity() - launcher.getTargetVelocity()) < 7, 25);

  // launch a ball
  intake.moveRelative(640, 200);
  delay(250);

  // drive forward and launch the second ball
  drive::dc.moveDistance(-13_in);
  delay(250);
  intake.moveRelative(1000, 200);
  delay(250);

  // stop launcher and intake
  launcher.moveVelocity(0);
  intake.moveVelocity(0);

  // turn and hit the bottom flag
  drive::dc.turnAngle(-60_deg);
  drive::dc.moveDistance(-5_in);
  drive::dc.turnAngle(55_deg);
  drive::dc.moveDistance(-16_in);

  // align on the wall
  drive::dc.left(-75);
  drive::dc.right(-75);
  delay(250);
  waitUntil(drive::left.getEfficiency() + drive::right.getEfficiency() < 8, 20);
  drive::dc.stop();

  // back away from the wall and turn towards the cap
  drive::dc.moveDistance(33_in);
  drive::dc.turnAngle(90_deg);

  // flip the cap
  intake.move(-127);
  drive::dc.moveDistance(-28_in);

  // back up from the cap
  drive::dc.moveDistance(22_in);
}
