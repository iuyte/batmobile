#include "devices.h"
#include "main.h"
#include "switcher.h"

/*Called when a button is released ot long pressed*/
static lv_res_t btnm_action(lv_obj_t *btnm, const char *txt) {
  printf("Button: %s released\n", txt);

  return LV_RES_OK; /*Return OK because the button matrix is not deleted*/
}

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

  // select the autonomous routine using the touchscreen
  chooseAuton();

  // a task that prints a lot of useful data to the LCD emulator
  pros::Task(
          [](void *none) {
            class Line {
            private:
              string         atext;
              AbstractMotor *motor;
              unsigned int   number;

            public:
              typedef enum { Velocity, Temperature, Position } ValueType;
              lv_obj_t *line;

              Line(string name, AbstractMotor *motor, ValueType valueType, unsigned int number) :
                  motor(motor), valueType(valueType), number(number) {
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

              string get() {
                switch (valueType) {
                case Velocity:
                  return atext + std::to_string(motor->getActualVelocity());
                case Temperature:
                  return atext + std::to_string(motor->getTemperature());
                case Position:
                  return atext + std::to_string(motor->getPosition());
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
                    Line("intake", &intake, Line::Velocity, ++nlines),
                    Line("lift", &lift, Line::Position, ++nlines),
                    Line("flipper", &flipper, Line::Position, ++nlines),
                    Line("left drive", &left, Line::Position, ++nlines),
                    Line("right drive", &right, Line::Position, ++nlines),
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

              light.set_value(abs(launcher.getActualVelocity() - launcher.getTargetVelocity()) >=
                              5);
              delay(50);
            }
          },
          nullptr);
}

void disabled() {}

void competition_initialize() {}
