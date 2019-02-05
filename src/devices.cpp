#include "devices.h"

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);

  namespace get {
    namespace drive {
      const float left() {
        return master.getAnalog(ControllerAnalog::leftY);
      }

      const float right() {
        return master.getAnalog(ControllerAnalog::leftY);
      }
      const bool holdToggle() { return master.getDigital(ControllerDigital::X); }
    } // namespace drive

    const float intake() {
      return master.getDigital(ControllerDigital::R1) -
             master.getDigital(ControllerDigital::R2);
    }

    const bool catapult() {
      static unsigned long lastTime = millis();

      if (millis() - lastTime > 200 && controller::master.getDigital(ControllerDigital::L1)) {
        lastTime = millis();
        return true;
      }

      return false;
    }
  } // namespace get
} // namespace controller

Motor intake(6, false, AbstractMotor::gearset::green);
Motor catapult(5, false, AbstractMotor::gearset::green);

namespace drive {
  MotorGroup left({Motor(1, false, AbstractMotor::gearset::green),
                   Motor(2, false, AbstractMotor::gearset::green)});
  MotorGroup right({Motor(3, true, AbstractMotor::gearset::green),
                    Motor(3, true, AbstractMotor::gearset::green)});

  void moveVelocity(double lvel, double rvel) {
    left.moveVelocity(lvel);
    right.moveVelocity(rvel);
  }

  ChassisControllerIntegrated dc = ChassisControllerFactory::create(
          left, right, AbstractMotor::gearset::green, ChassisScales({4.1_in, 15.125_in}));
  AsyncMotionProfileController dpc = AsyncControllerFactory::motionProfile(.5, 1.25, 8, dc);
} // namespace drive
