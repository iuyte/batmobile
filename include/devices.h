#ifndef _IUYTE_DEVICES_H_
#define _IUYTE_DEVICES_H_

#include "main.h"

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
} // namespace controller

/** different buttons and controls for operator control */
namespace operatorCmd {
  namespace drive {
    const float left();
    const float right();
    const bool  holdToggle();
  } // namespace drive

  const float lift();
  const float intake();

  namespace launcher {
    const bool off();
    const bool backwards();
    const bool less();
    const bool more();
    const bool middleFlag();
    const bool highFlag();
  } // namespace launcher
} // namespace operatorCmd

/** things having to do with the drive */
namespace drive {
  extern Motor left;
  extern Motor right;

  /** move the drive at a specific velocity */
  void moveVelocity(double lvel, double rvel);

  /** drive chassis profile controller, a combination of the left and right side motors */
  extern ChassisControllerIntegrated dc;
  /** motion profile controller for the drive */
  extern AsyncMotionProfileController dpc;
} // namespace drive

extern Motor intake;
/** combination of both lift motors */
extern MotorGroup lift;
/** combination of both flywheel motors */
extern MotorGroup launcher;

/** status lights */
extern pros::ADIDigitalOut light;

#endif
