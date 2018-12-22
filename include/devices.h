#ifndef _IUYTE_DEVICES_H_
#define _IUYTE_DEVICES_H_

#include "main.h"
#include "util.h"

using namespace okapi::literals;
using namespace okapi;
using pros::delay;
using pros::millis;

/** main V5 controller */
extern Controller controller;
extern Motor      left, right, intake, flipper;
/** combination of both lift/flywheel motors */
extern MotorGroup lift, launcher;
/** combination of the left and right side motors */
extern ChassisControllerIntegrated drive;
/** motion profile controller for the drive */
extern AsyncMotionProfileController dc;

/** status lights */
extern pros::ADIDigitalOut lights[2];

#endif
