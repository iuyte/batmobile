#include "devices.h"
#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // LCD emulator init
  pros::lcd::initialize();
  pros::lcd::set_text(0, "RoboEagles: 709S");

  pros::Task(
          [](void *param) {
            while (true) {
              pros::lcd::print(1, "drive position: %f, %f", left.getPosition(), right.getPosition());
              pros::lcd::print(2,
                         "drive velocity: %f, %f",
                         left.getActualVelocity(),
                         right.getActualVelocity());
              pros::lcd::print(3, "launcher position: %f", launcher.getPosition());
              pros::lcd::print(4, "launcher torque: %f", launcher.getTorque());
              pros::lcd::print(5, "launcher temp: %f", launcher.getTemperature());
              // controller.setText(1, 1, std::to_string(launcher.getPosition()));
              delay(75);
            }
          },
          nullptr);

  // wait for something to happen
  while (pros::competition::is_disabled())
    delay(50);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // drive controller init
  dc.generatePath({okapi::Point{0_ft, 0_ft, 0_deg},
                   okapi::Point{0_ft, sqrt(2) * okapi::foot, 0_deg},
                   okapi::Point{0_ft, sqrt(2) * okapi::foot, -180_deg}},
                  "A");

  // wait for something to happen
  while (pros::competition::is_disabled())
    delay(50);
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
