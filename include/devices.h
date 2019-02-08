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

  /** different buttons and controls for operator control */
  namespace get {
    namespace drive {
      const float left();
      const float right();
      const bool  holdToggle();
    } // namespace drive

    const float intake();

    const int catapult();
  } // namespace get
} // namespace controller

/** things having to do with the drive */
namespace drive {
  extern MotorGroup left;
  extern MotorGroup right;

  /** move the drive at a specific velocity */
  void moveVelocity(double lvel, double rvel);

  /** drive chassis profile controller, a combination of the left and right side motors */
  extern ChassisControllerIntegrated dc;
  /** motion profile controller for the drive */
  extern AsyncMotionProfileController dpc;
} // namespace drive

/** intake */
extern Motor intake;
/** catapult */
extern Motor catapult;
/** catapult potentiometer */
extern Potentiometer cataPot;

#endif
