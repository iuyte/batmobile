#ifndef _IUYTE_DEVICES_H_
#define _IUYTE_DEVICES_H_

#include "main.h"
#include "util.h"

using namespace okapi::literals;
using namespace okapi;
using pros::delay;

/** main V5 controller */
extern okapi::Controller controller;
extern okapi::Motor      left, right, launcher, intake;
/** combination of both lift motors */
extern okapi::MotorGroup lift;
/** launcher async controller */
extern AsyncPosIntegratedController launchC;
/** combination of the left and right side motors */
extern okapi::ChassisControllerIntegrated drive;
/** motion profile controller for the drive */
extern okapi::AsyncMotionProfileController dc;

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
