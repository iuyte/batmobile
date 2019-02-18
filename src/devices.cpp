#include "devices.h"

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);
  Controller partner(ControllerId::partner);

  namespace get {
    namespace drive {
      const float forward() {
        return master.getAnalog(ControllerAnalog::leftY) -
               partner.getAnalog(ControllerAnalog::leftY);
      }

      const float turn() {
        return master.getAnalog(ControllerAnalog::rightX) -
               partner.getAnalog(ControllerAnalog::rightX);
      }

      const float strafe() {
        return master.getAnalog(ControllerAnalog::leftX) -
               partner.getAnalog(ControllerAnalog::leftX);
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
  Motor motors[2][2] {{Motor(16, false, AbstractMotor::gearset::green),
                     Motor(9, false, AbstractMotor::gearset::green)},
                    {Motor(15, true, AbstractMotor::gearset::green),
                     Motor(10, true, AbstractMotor::gearset::green)}};
  MotorGroup left({motors[0][0], motors[0][1]});
  MotorGroup right({motors[1][0], motors[1][1]});

  void moveVelocity(double lvel, double rvel) {
    left.moveVelocity(lvel);
    right.moveVelocity(rvel);
  }

  void control(float forward, float turn, float strafe) {
    #define dmax 200
    motors[0][0].moveVelocity(trim(dmax * (forward + turn + strafe), -dmax, dmax));
    motors[0][1].moveVelocity(trim(dmax * (forward + turn - strafe), -dmax, dmax));
    motors[1][0].moveVelocity(trim(dmax * (forward - turn - strafe), -dmax, dmax));
    motors[1][1].moveVelocity(trim(dmax * (forward - turn + strafe), -dmax, dmax));
  }

  ChassisControllerIntegrated dc = ChassisControllerFactory::create(
          left, right, AbstractMotor::gearset::green, ChassisScales({4.15_in, 14_in}));
  AsyncMotionProfileController dpc = AsyncControllerFactory::motionProfile(3, 2, 10, dc);
} // namespace drive
