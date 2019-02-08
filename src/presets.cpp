#include "presets.h"

CataState   cataState = CataState::neutral;
pros::Task *catapultTaskHandle;

void catapultTask(void *none) {
  // runs the task at a constant rate
  Rate rate;

  // the current state
  CataState achieved = CataState::neutral;

  // position to hold
  float pos = 0;

  // start off the catapult as still
  catapult.moveVelocity(0);

  while (true) {
    if (cataState != achieved) {
      switch (cataState) {
      case CataState::ready:
        catapult.moveVelocity(100);
        waitUntil(cataPot.get() < cataPos[HIGH], 20);
        catapult.moveVelocity(75);
        waitUntil(cataPot.get() < cataPos[LOW] + 100, 20);
        pos = catapult.getPosition();
        catapult.moveAbsolute(pos, 5);
        break;
      case CataState::fire:
        catapult.moveVelocity(100);
        waitUntil(cataPot.get() < cataPos[LOW], 20);
        catapult.moveRelative(1000, 100);
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

CataState toggleState(CataState newState) {
  if (newState == CataState::neutral) {
    cataState = (cataState == ready) ? fire : ready;
  } else {
    cataState = newState;
  }

  catapultTaskHandle->notify();
}

void catapultMove(CataState s) {}
