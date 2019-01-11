#ifndef _IUYTE_DEVICES_H_
#define _IUYTE_DEVICES_H_

#include "main.h"
#include "util.h"

using namespace okapi::literals;
using namespace okapi;
using pros::delay;
using pros::millis;

/** V5 controllers and buttons */
namespace controller {
  /** main V5 controller */
  extern Controller master;
  /** partner joystick V5 controller */
  extern Controller partner;

  const float leftDrive();
  const float rightDrive();
  const bool  driveHoldToggle();
  const bool  lift();
  const bool  flipper();
  const bool  intake();

  namespace launcher {
    const bool toggle();
    const bool backwards();
    const bool less();
    const bool more();
    const bool middleFlag();
    const bool highFlag();
  } // namespace launcher
} // namespace controller

extern Motor left, right, intake, flipper;
/** combination of both lift motors */
extern MotorGroup lift;
/** combination of both flywheel motors */
extern MotorGroup launcher;
/** combination of the left and right side motors */
extern ChassisControllerIntegrated drive;
/** motion profile controller for the drive */
extern AsyncMotionProfileController dc;

/** status lights */
extern pros::ADIDigitalOut light;

#endif
