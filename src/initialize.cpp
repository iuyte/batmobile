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

  // Clear controller
  controller.clear();

  // set flywheel and intake to "coast" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  launcher.setBrakeMode(AbstractMotor::brakeMode::coast);
  // set the lift, and flipper to "hold" mode
  lift.setBrakeMode(AbstractMotor::brakeMode::hold);
  flipper.setBrakeMode(AbstractMotor::brakeMode::hold);

  // pot init
  pot.zero = 1550;

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(
          [](void *none) {
            while (true) {
              pros::lcd::print(
                      1, "drive position: %f, %f", left.getPosition(), right.getPosition());
              pros::lcd::print(2,
                               "drive velocity: %f, %f",
                               left.getActualVelocity(),
                               right.getActualVelocity());
              pros::lcd::print(3,
                               "launcher position: %f, %f",
                               launcher.getPosition(),
                               launcher.getActualVelocity());
              pros::lcd::print(4, "launcher torque: %f", launcher.getTorque());
              pros::lcd::print(5, "launcher temp: %f", launcher.getTemperature());
              pros::lcd::print(6, "flipper position: %f", flipper.getPosition());
              pros::lcd::print(7, "indicator: %d", indicator);
              controller.setText(0, 0, std::string("v: ").append(std::to_string(launcher.getActualVelocity())));
              delay(50);
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
  // drive controller init: generate the motion path now (takes time) such that less precious time
  // during autonomous is lost
  dc.generatePath({Point{0_ft, 0_ft, 0_deg},
                   Point{0_ft, sqrt(2) * foot, 0_deg},
                   Point{0_ft, sqrt(2) * foot, -180_deg}},
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
