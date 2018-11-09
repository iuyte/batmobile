#include "devices.h"
#include "main.h"
#include "presets.h"
#include "util.h"

void autonomous();

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // The maximum speed in RPM for the drive motors
  const float dmax = 185;

  // set the drive and intake to "coast" mode, as it is more natural for drivers
  okapi::AbstractMotor::brakeMode bmode = okapi::AbstractMotor::brakeMode::coast;
  left.setBrakeMode(bmode);
  right.setBrakeMode(bmode);
  intake.setBrakeMode(okapi::AbstractMotor::brakeMode::coast);
  // set the launcher and lift to "hold" mode
  launcher.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);
  lift.setBrakeMode(okapi::AbstractMotor::brakeMode::hold);

  // infinite launcher control task
  auto launcherTask = pros::Task(
          [](void *none) {
            // state switching var
            int t;
            // power
            float lp;

            while (!pros::competition::is_disabled() && !pros::competition::is_autonomous()) {
              if (lp) {
                launcher.moveVoltage(lp);
              } else {
                t = controller.getDigital(okapi::ControllerDigital::Y) * 2 +
                    controller.getDigital(okapi::ControllerDigital::X) * 3;
                switch (t) {
                case 2:
                  // pull the launcher arm back
                  launcherReady();
                  indicator = 100;
                  break;
                case 3:
                  // fire the catapult
                  launcherFire();
                  indicator = -494;
                  break;
                default:
                  break;
                }
              }
              delay(25);
            }
          },
          nullptr);

  // infinite driver-control loop, runs: drive, intake, and launcher
  while (true) {
    left.moveVelocity(controller.getAnalog(okapi::ControllerAnalog::leftY) * dmax +
                      controller.getDigital(okapi::ControllerDigital::up) * dmax -
                      controller.getDigital(okapi::ControllerDigital::down) * dmax);
    right.moveVelocity(controller.getAnalog(okapi::ControllerAnalog::rightY) * dmax +
                       controller.getDigital(okapi::ControllerDigital::up) * dmax -
                       controller.getDigital(okapi::ControllerDigital::down) * dmax);

    intake.move(127 * controller.getDigital(okapi::ControllerDigital::R1) -
                127 * controller.getDigital(okapi::ControllerDigital::R2));
    lift.moveVelocity(200 * controller.getDigital(okapi::ControllerDigital::A) -
                      200 * controller.getDigital(okapi::ControllerDigital::B));

    delay(25);
  }
}

void launcherMove(float voltage) {
  // distance that the gear turns in clicks
  static const float gDis = 625;
  // launcher home position
  float lph = 0;

  if (abs(launcher.getPosition() - lph) < 400)
    // if the launcher has a high torque, it means that the slip-gear is now driving the gear
    // the catapult is on, and if this is new, then the home position should be reset
    if (launcher.getTorque() > 0.2) {
      lph = (abs(launcher.getPosition() - lph) < 400) ? lph : launcher.getPosition();
    }
  if (voltage) { // if there is controller input for the launcher
    launcher.move(voltage);
  } else { // if there is no controller input
    // if the catapult arm is in the process of being pulled back, hold its position, and if not,
    // don't bother holding the position
    if (launcher.getPosition() - lph < gDis)
      launcher.moveVelocity(0);
    else
      launcher.move(0);
  }
}
