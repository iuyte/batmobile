#include "devices.h"

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);

  namespace get {
    namespace drive {
      const float left() { return master.getAnalog(ControllerAnalog::leftY); }

      const float right() { return master.getAnalog(ControllerAnalog::rightY); }
      const bool  holdToggle() { return master.getDigital(ControllerDigital::X); }
    } // namespace drive

    const float intake() {
      return master.getDigital(ControllerDigital::R1) - master.getDigital(ControllerDigital::R2);
    }

    const int catapult() {
      static unsigned long lastTime = millis();

      if (millis() - lastTime > 200 && controller::master.getDigital(ControllerDigital::L1)) {
        lastTime = millis();
        return 1;
      } else if (controller::master.getDigital(ControllerDigital::L2)){
        return -1;
      }

      return 0;
    }
  } // namespace get
} // namespace controller

Motor         intake(12, true, AbstractMotor::gearset::green);
Motor         catapult(21, false, AbstractMotor::gearset::green);
Potentiometer cataPot('a');

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
          left, right, AbstractMotor::gearset::green, ChassisScales({4.1_in, 15.125_in}));
  AsyncMotionProfileController dpc = AsyncControllerFactory::motionProfile(.5, 1.25, 8, dc);
} // namespace drive
