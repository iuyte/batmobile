#include "presets.h"

CataState   cataState = CataState::neutral;
pros::Task *catapultTaskHandle;
int         switchable = 1;

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
      switchable = 0;

      switch (cataState) {
      case CataState::ready:
        catapult.setBrakeMode(AbstractMotor::brakeMode::hold);
        catapult.moveVelocity(100);
        waitUntil(cataPot.get() < cataPos[1], 20);
        catapult.moveVelocity(75);
        waitUntil(cataPot.get() < cataPos[0], 20);
        pos = catapult.getPosition();
        catapult.moveAbsolute(pos, 5);
        break;
      case CataState::fire:
        catapult.moveVelocity(100);
        waitUntil(cataPot.get() > cataPos[1], 20);
        waitUntil(cataPot.get() < cataPos[2], 20);
        catapult.setBrakeMode(AbstractMotor::brakeMode::coast);
        break;
      default:
        break;
      }

      achieved = cataState;
    }

    switchable = 1;
    catapult.moveVelocity(0);
    pros::c::task_notify_take(true, -1);
  }
}

bool catapultAchieved() {
  return (bool)switchable;
}

CataState toggleState(CataState newState) {
  if (switchable) {
    if (newState == CataState::neutral) {
      cataState = (cataState == ready) ? fire : ready;
    } else {
      cataState = newState;
    }

    catapultTaskHandle->notify();
  }
}
