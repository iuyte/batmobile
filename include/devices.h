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
class Pot : pros::ADIPotentiometer {
public:
  using pros::ADIPotentiometer::ADIPotentiometer;

  /** reset the sensor's home position */
  void reset();

  /** set the scale (starts as 1.0). Ex: if scale = -.5, the values are halved and inverted. */
  void setScale(double scalv);

  /** get the sensor value with zero and scale applied */
  double get();

  double z;
  double scale = 1.0;
};

/** Potentiometer in legacy ADI port H */
extern ::Pot pot;

#endif
