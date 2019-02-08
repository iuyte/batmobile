#include "presets.h"

CataState cataState = CataState::neutral;

void catapultTask(void *none) {
  // runs the task at a constant rate
  Rate rate;

  // the current state
  CataState achieved = CataState::neutral;

  // position to hold
  float pos = 0;

  // start off the catapult as still
  catapult.moveVelocity(0);

  while (!(pros::competition::is_disabled() || pros::competition::is_autonomous())) {
    if (cataState != achieved) {
      switch (cataState) {
      case CataState::ready:
        catapult.moveVelocity(200);
        waitUntil(cataPot.get() < cataPos[HIGH], 20);
        catapult.moveVelocity(150);
        waitUntil(cataPot.get() < cataPos[LOW], 20);
        pos = catapult.getPosition();
        catapult.moveAbsolute(pos, 25);
        break;
      case CataState::fire:
        catapult.moveVelocity(200);
        waitUntil(cataPot.get() < cataPos[LOW], 20);
        catapult.moveRelative(180, 200);
        waitUntil(motorPosTargetReached(catapult, 10), 20);
        break;
      default:
        break;
      }

      achieved = cataState;
      catapult.moveVelocity(0);
    }

    pros::c::task_notify_take(true, -1);
  }
}

CataState toggleState() { cataState = (cataState == fire) ? ready : fire; }
