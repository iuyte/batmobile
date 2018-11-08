#ifndef _IUYTE_DEVICES_H_
#define _IUYTE_DEVICES_H_

#include "main.h"
using namespace okapi::literals;
using namespace okapi;
using pros::delay;

/** The main V5 controller */
extern okapi::Controller controller;
extern okapi::Motor      left, right, launcher, intake;
/** The combination of the left and right side motors */
extern okapi::ChassisControllerIntegrated drive;
/** The motion profile controller  for the drive */
extern okapi::AsyncMotionProfileController dc;

#endif
