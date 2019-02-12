#include "devices.h"

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);
  Controller partner(ControllerId::partner);

  namespace get {
    namespace drive {
      const float left() {
        return master.getAnalog(ControllerAnalog::leftY) -
               partner.getAnalog(ControllerAnalog::rightY);
      }
      const float right() {
        return master.getAnalog(ControllerAnalog::rightY) -
               partner.getAnalog(ControllerAnalog::leftY);
      }
      const bool holdToggle() { return master.getDigital(ControllerDigital::X); }
    } // namespace drive

    const float intake() {
      return master.getDigital(ControllerDigital::R1) - master.getDigital(ControllerDigital::R2);
    }

    const int catapult() {
      static unsigned long lastTime = millis();

      if (millis() - lastTime > 350) {
        if (controller::master.getDigital(ControllerDigital::L1)) {
          lastTime = millis();
          return 1;
        } else if (controller::master.getDigital(ControllerDigital::L2)) {
          lastTime = millis();
          return -1;
        }
      }

      return 0;
    }
  } // namespace get
} // namespace controller

Motor intake(12, true, AbstractMotor::gearset::green);

namespace catapult {
  MotorGroup    motor({Motor(21, false, AbstractMotor::gearset::red),
                    Motor(20, true, AbstractMotor::gearset::red)});
  Potentiometer pot('a');
} // namespace catapult

namespace drive {
  MotorGroup left({Motor(16, false, AbstractMotor::gearset::green),
                   Motor(9, false, AbstractMotor::gearset::green)});
  MotorGroup right({Motor(15, true, AbstractMotor::gearset::green),
                    Motor(10, true, AbstractMotor::gearset::green)});

  void moveVelocity(double lvel, double rvel) {
    left.moveVelocity(lvel);
    right.moveVelocity(rvel);
  }

  ChassisControllerIntegrated dc = ChassisControllerFactory::create(
          left, right, AbstractMotor::gearset::green, ChassisScales({4.15_in, 14_in}));
  AsyncMotionProfileController dpc = AsyncControllerFactory::motionProfile(1, 2, 10, dc);
} // namespace drive
