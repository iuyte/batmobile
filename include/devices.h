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
/** velocity controller for the flywheel */
extern AsyncVelPIDController fc;

#endif
