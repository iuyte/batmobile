#include "devices.h"
#include "main.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Clear controller
  controller.clear();

  // set flywheel and intake to "coast" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  launcher.setBrakeMode(AbstractMotor::brakeMode::coast);
  // set the lift, and flipper to "hold" mode
  lift.setBrakeMode(AbstractMotor::brakeMode::hold);
  flipper.setBrakeMode(AbstractMotor::brakeMode::hold);

  // set the flywheel pid
  //launcher.setVelPID(.4, .1, 0, .75);
  launcher.setVelPID(0, 2, 0, 7);

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(
          [](void *none) {
            // create a Label on the V5 screen
            lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);

            // change the Label's text
            lv_label_set_text(label1, "Hello world!");

            // Align the Label to the center
            lv_obj_align(label1, NULL, LV_ALIGN_CENTER, -50, 0);

            // start of text
            auto text = std::string("speed: ");

            while (true) {
              lv_label_set_text(label1, (text + std::to_string(launcher.getActualVelocity())).c_str());
              controller.setText(
                      2,
                      0,
                      std::string("v: ")
                              .append(std::to_string((int)(launcher.getActualVelocity() + .5)))
                              .append("   "));
              delay(50);
            }
          },
          nullptr);
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
