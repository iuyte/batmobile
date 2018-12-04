#ifndef _IUYTE_DEVICES_H_
#define _IUYTE_DEVICES_H_

#include "main.h"
#include "util.h"

using namespace okapi::literals;
using namespace okapi;
using pros::delay;

/** main V5 controller */
extern Controller controller;
extern Motor      left, right, intake, flipper;
/** combination of both lift/flywheel motors */
extern MotorGroup lift, launcher;
/** combination of the left and right side motors */
extern ChassisControllerIntegrated drive;
/** motion profile controller for the drive */
extern AsyncMotionProfileController dc;

/** my own potentiometer class */
struct Pot : public pros::ADIPotentiometer {
  using pros::ADIPotentiometer::ADIPotentiometer;

  /** get the sensor value with zero and scale applied */
  int32_t get();

  int32_t zero;
  int32_t scale = 1.0;
};

/** Potentiometer in legacy ADI port H */
extern ::Pot pot;

#endif
