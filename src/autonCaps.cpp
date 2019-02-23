#include "autonomous.h"
#include "util.h"

#define side_red -1
#define side_blue 1

void autonScoreCap() {
  // set the intake to on
  intake.move(127);

  // drive forward, pick up the ball, and back up
  drive::dpc.setTarget("F1");
  drive::dpc.waitUntilSettled();
  drive::dpc.setTarget("6", true);
  drive::dpc.waitUntilSettled();

  // move away from the platforms to allow room to turn
  drive::strafe(600, 110);
  drive::waitUntilCompletion();

  // turn
  drive::moveRelative(555, -555, 110);
  drive::waitUntilCompletion();

  // get on the lower platform
  drive::left.moveRelative(-800, 200);
  drive::right.moveRelative(-800, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);
  drive::moveVelocity(0, 0);

  // stop the intake
  intake.move(0);
}

void autonCaps(int side) { autonScoreCap(); }

void autonRedCaps() { autonCaps(side_red); }

void autonBlueCaps() { autonCaps(side_blue); }
