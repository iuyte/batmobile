#ifndef _IUYTE_DEVICES_H_
#define _IUYTE_DEVICES_H_

#include "main.h"
#include <memory>

using namespace okapi::literals;
using namespace okapi;
using pros::delay;
using pros::millis;
using std::shared_ptr;

/** V5 controllers and buttons */
namespace controller {
  /** main V5 controller */
  extern Controller master;

  /** different buttons and controls for operator control */
  namespace get {
    namespace drive {
      const float forward();
      const float turn();
      const float strafe();
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

  /** get the current angle of the drive, in degrees */
  double getAngle();

  /** reset the sensors */
  void reset();

  /** whether or not the drive motors have each reached their position targets */
  bool atTarget(int range = 15);

  /** the total velocity of the drive motors */
  double totalVelocity();

  /** strafe to a certain amount of ticks */
  void strafe(float ticks, int vel = 200);

  /** move to relative positions */
  void moveRelative(float l, float r, int vel);

  /** move to absolute positions */
  void moveAbsolute(float l, float r, int vel);

  void waitUntilCompletion(unsigned long minTime = 0);
  void waitUntilStarted();
  void waitUntilStopped();

  /** move the drive at a specific velocity */
  void moveVelocity(double lvel, double rvel);

  /** control all aspects of the drive on a scale of -1 to +1 */
  void control(float forward, float turn, float strafe);

  /** turn the robot a number of degrees */
  void turn(float angle, float range = 2, bool absolute = false);

  /** drive chassis profile controller, a combination of the left and right side motors */
  extern shared_ptr<ChassisController> dc;
  /** motion profile controller for the drive */
  extern shared_ptr<AsyncMotionProfileController> dpc;
} // namespace drive

/** catapult */
namespace catapult {
  /** catapult motors */
  extern MotorGroup motor;

  /** catapult potentiometer */
  extern Potentiometer pot;
} // namespace catapult

/** intake */
extern Motor intake;

/** vision sensor */
extern pros::Vision vision;

#endif
