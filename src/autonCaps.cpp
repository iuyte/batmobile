#include "autonomous.h"
#include "util.h"

#define side_red -1
#define side_blue 1

void autonScoreCap() {
  intake.move(127);

  drive::dc.setMaxVelocity(115);
  drive::dc.moveDistance(-39_in);

  intake.moveRelative(1260, 200);
  intake.moveRelative(-300, 100);

  auto a = (drive::left.getPosition() + drive::right.getPosition()) / 2;
  drive::left.moveAbsolute(a, 75);
  drive::right.moveAbsolute(a, 75);

  drive::dc.moveDistance(24_in);
}

void autonCaps(int side) { autonScoreCap(); }

void autonRedCaps() { autonCaps(side_red); }

void autonBlueCaps() { autonCaps(side_blue); }
