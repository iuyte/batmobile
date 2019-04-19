#include "devices.h"
#include "autonomous.h"
#include "util.h"

void autonTest() {
  // align along the x
  vision::back.alignX(vision::Alignment::Platform);
  delay(200);

  // align along the y
  vision::back.alignY(vision::Alignment::Platform, 25);

  // get onto the platform
  drive::reset();
  drive::dc->setMaxVelocity(200);
  drive::moveAbsolute(-2200, -2200, 200);
  waitUntil(motorPosTargetReached(drive::left, 50) && motorPosTargetReached(drive::right, 50), 20);

  drive::left.setBrakeMode(AbstractMotor::brakeMode::hold);
  drive::right.setBrakeMode(AbstractMotor::brakeMode::hold);
  drive::moveVelocity(0, 0);
}
