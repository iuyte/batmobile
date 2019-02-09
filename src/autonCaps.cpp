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
  drive::dpc.setTarget("F2", true);
  drive::dpc.waitUntilSettled();

  // stop the intake
  intake.move(0);
}

void autonCaps(int side) { autonScoreCap(); }

void autonRedCaps() { autonCaps(side_red); }

void autonBlueCaps() { autonCaps(side_blue); }
