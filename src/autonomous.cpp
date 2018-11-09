#include "devices.h"
#include "main.h"
#include "presets.h"
#include "util.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
  // make it so the drive motors hold commanded positions, rather than coast like in driver-control
  left.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);
  right.setBrakeMode(okapi::AbstractMotor::brakeMode::brake);

  dc.setTarget("A");     // set the target as the first motion path
  dc.waitUntilSettled(); // let the drive complete the motion path
  delay(150);
  // drive.turnAngle(-135); // turn mostly around counterclockwise
}
