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
  // launcher.setVelPID(.4, .1, 0, .75);
  launcher.setVelPID(0, 2, 0, 7);

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(
          [](void *none) {
            class Line {
            private:
              std::string    atext;
              AbstractMotor *motor;
              unsigned int   number;

            public:
              typedef enum { Velocity, Temperature, Position } ValueType;
              lv_obj_t *line;

              Line(std::string    name,
                   AbstractMotor *motor,
                   ValueType      valueType,
                   unsigned int   number) :
                  motor(motor),
                  valueType(valueType), number(number) {
                switch (valueType) {
                case Velocity:
                  atext = name + " velocity (rpm): ";
                  break;
                case Temperature:
                  atext = name + " temperature (°): ";
                  break;
                case Position:
                  atext = name + " position: ";
                  break;
                default:
                  break;
                }

                line = lv_label_create(lv_scr_act(), NULL);
                delay(2);
              }

              std::string get() {
                switch (valueType) {
                case Velocity:
                  return atext + std::to_string(motor->getActualVelocity());
                case Temperature:
                  return atext + std::to_string(motor->getTemperature());
                case Position:
                  return atext + std::to_string(motor->getPosition());
                default:
                  return std::string("error");
                }
              }

            private:
              ValueType valueType;
            };

            unsigned int nlines = 0;

            Line lines[] = {
                    Line("flywheel", &launcher, Line::Velocity, nlines++),
                    Line("flywheel", &launcher, Line::Temperature, nlines++),
                    Line("intake", &intake, Line::Velocity, nlines++),
                    Line("lift", &lift, Line::Position, nlines++),
                    Line("flipper", &flipper, Line::Position, nlines++),
            };

            for (std::size_t i = 0; i < nlines; i++) {
              lv_obj_align(lines[i].line, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20 * i);
            }

            while (true) {
              for (std::size_t i = 0; i < nlines; i++) {
                lv_label_set_text(lines[i].line, lines[i].get().c_str());
              }

              controller.setText(
                      2, 0, std::to_string((int)(launcher.getActualVelocity() + .5)).append("   "));
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
  dc.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{0_ft, sqrt(2) * foot, 0_deg},
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
