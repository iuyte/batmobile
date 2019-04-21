#include "autonomous.h"
#include "devices.h"
#include "util.h"

void autonTest() {
  intake.move(127);
  arm.moveAbsolute(ArmP::Low, 200);
  delay(500);
}
