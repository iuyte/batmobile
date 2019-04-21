#include "catapult.h"

namespace catapult {
  State       state = State::None;
  pros::Task *task;
  bool        _atTarget = true;

  void manager(void *None) {
    // runs the task at a constant rate
    Rate rate;

    // the current state
    State achieved = State::None;

    // position to hold
    float pos = 0;

    // start off the motor as still
    motor.moveVelocity(0);

    while (true) {
      if (state != achieved) {
        _atTarget = false;

        switch (state) {
        case State::Ready:
          motor.setBrakeMode(AbstractMotor::brakeMode::hold);
          motor.moveVelocity(200);
          waitUntiloD(pot.get() < presets[1], 20);
          motor.moveVelocity(150);
          waitUntiloD(pot.get() < presets[0], 20);
          pos = motor.getPosition();
          motor.moveAbsolute(pos, 20);
          break;
        case State::Fire:
          motor.moveVelocity(100);
          waitUntiloD(pot.get() > presets[1], 20);
          motor.moveRelative(2.8 * imev5BlueTPR, 100);
          waitUntiloD(motorPosTargetReached(motor, 20), 20);
          motor.setBrakeMode(AbstractMotor::brakeMode::coast);
          motor.moveVelocity(0);
          break;
        default:
          motor.moveVelocity(0);
          break;
        }

        achieved = state;
      }

      _atTarget = true;

      pros::Task::current().notify_take(true, -1);
    }
  }

  pros::Task *startManager() {
    static pros::Task handle(&manager, nullptr);
    task = &handle;
    return task;
  }

  bool atTarget() { return _atTarget || (state == State::None); }

  State move(State newState) {
    if (_atTarget) {
      if (newState == State::None) {
        state = (state == Ready) ? Fire : Ready;
      } else {
        state = newState;
      }

      task->notify();
    }

    return state;
  }

  void ready() {
    waitUntil(atTarget(), 20);
    move(State::Ready);
  }

  void fire() {
    waitUntil(atTarget(), 20);
    move(State::Fire);
  }
} // namespace catapult
