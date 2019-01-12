#include "main.h"

bool isSelected = false;

void initialize() {
  // Clear controller
  controller::master.clear();
  controller::partner.clear();

  // set flywheel and intake to "coast" mode
  intake.setBrakeMode(AbstractMotor::brakeMode::coast);
  launcher.setBrakeMode(AbstractMotor::brakeMode::coast);
  // set the lift, and flipper to "hold" mode
  lift.setBrakeMode(AbstractMotor::brakeMode::hold);
  flipper.setBrakeMode(AbstractMotor::brakeMode::hold);

  // set the flywheel pid
  launcher.setVelPID(0, 2, 0, 7);

  // select the autonomous routine using the touchscreen
  chooseAuton();
  isSelected = true;

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(
          [](void *none) {
            // wait until the autonmous is selected or the robot is enabled
            // waitUntil(isSelected || !pros::competition::is_disabled(), 20);

            class Line {
            private:
              string         atext;
              AbstractMotor *motor;
              unsigned int   number;

            public:
              typedef enum { Position, Temperature, Velocity, Power } ValueType;
              lv_obj_t *line;

              Line(string name, AbstractMotor *motor, ValueType valueType, unsigned int number) :
                  motor(motor), valueType(valueType), number(number) {
                switch (valueType) {
                case Position:
                  atext = name + " position: ";
                  break;
                case Temperature:
                  atext = name + " temperature (°): ";
                  break;
                case Velocity:
                  atext = name + " velocity (rpm): ";
                  break;
                case Power:
                  atext = name + " power: ";
                  break;
                default:
                  break;
                }

                line = lv_label_create(lv_scr_act(), NULL);
                delay(2);
              }

              string get() {
                switch (valueType) {
                case Position:
                  return atext + std::to_string(motor->getPosition());
                case Temperature:
                  return atext + std::to_string(motor->getTemperature());
                case Velocity:
                  return atext + std::to_string(motor->getActualVelocity());
                case Power:
                  return atext + std::to_string(motor->getPower() * (127 / 11));
                default:
                  return string("error");
                }
              }

            private:
              ValueType valueType;
            };

            unsigned int nlines = 0;

            Line lines[] = {
                    Line("flywheel", &launcher, Line::Velocity, ++nlines),
                    Line("flywheel", &launcher, Line::Temperature, ++nlines),
                    Line("flywheel", &launcher, Line::Power, ++nlines),
                    Line("intake", &intake, Line::Velocity, ++nlines),
                    Line("lift", &lift, Line::Position, ++nlines),
                    Line("flipper", &flipper, Line::Position, ++nlines),
                    Line("left drive", &left, Line::Position, ++nlines),
                    Line("right drive", &right, Line::Position, ++nlines),
            };

            for (std::size_t i = 0; i < nlines; i++) {
              lv_obj_align(lines[i].line, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20 * i);
            }

            char tline[20];

            while (true) {
              for (std::size_t i = 0; i < nlines; i++) {
                lv_label_set_text(lines[i].line, lines[i].get().c_str());
              }

              sprintf(tline, "v: %.3d | t: %.2f", (int)(launcher.getActualVelocity() + .5),
                      launcher.getTemperature());
              controller::master.setText(2, 0, tline);
              controller::partner.setText(2, 0, tline);

              light.set_value(abs(launcher.getActualVelocity() - launcher.getTargetVelocity()) >
                                      5 ||
                              launcher.getTargetVelocity() == 0);
              delay(50);
            }
          },
          nullptr);
}

void disabled() {}

void competition_initialize() {}
