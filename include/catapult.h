#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "main.h"

namespace catapult {
  /** catapult position presets */
  static const float presets[] = {1550, 2300, 2350};

  /** catapult states */
  enum State{
          None,
          Ready,
          Fire,
  };

  /** change catapult state */
  State move(State newState = State::None);

  /** whether or not the catapult had reached it's goal */
  bool atTarget();

  /** task to manage the position of the catapult */
  pros::Task *startManager();

  /** ready the catapult */
  void ready();

  /** fire the catapult */
  void fire();
}

#endif
