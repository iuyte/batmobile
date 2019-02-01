#include "devices.h"

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);
  /** partner joystick V5 controller */
  Controller partner(ControllerId::partner);
} // namespace controller

namespace operatorCmd {
  namespace drive {
    const float left() {
      return controller::partner.getAnalog(ControllerAnalog::leftY) -
             controller::master.getAnalog(ControllerAnalog::rightY);
    }

    const float right() {
      return controller::partner.getAnalog(ControllerAnalog::rightY) -
             controller::master.getAnalog(ControllerAnalog::leftY);
    }
    const bool holdToggle() { return controller::partner.getDigital(ControllerDigital::X); }
  } // namespace drive

  const float lift() {
    return controller::partner.getDigital(ControllerDigital::L1) - controller::partner.getDigital(ControllerDigital::L2);
  }

  const float intake() {
    return controller::master.getDigital(ControllerDigital::R1) - controller::master.getDigital(ControllerDigital::R2);
  }

  namespace launcher {
    const bool off() { return controller::master.getDigital(ControllerDigital::A); }

    const bool backwards() { return controller::master.getDigital(ControllerDigital::B); }

    const bool less() { return controller::master.getDigital(ControllerDigital::down); }

    const bool more() { return controller::master.getDigital(ControllerDigital::up); }

    const bool middleFlag() { return controller::master.getDigital(ControllerDigital::L2); }

    const bool highFlag() { return controller::master.getDigital(ControllerDigital::L1); }
  } // namespace launcher
} // namespace operatorCmd

Motor      intake(13, false, AbstractMotor::gearset::green);
MotorGroup lift({Motor(15, false, AbstractMotor::gearset::green),
                 Motor(14, true, AbstractMotor::gearset::green)});
MotorGroup launcher({Motor(10, false, AbstractMotor::gearset::green),
                     Motor(19, false, AbstractMotor::gearset::green),
                     Motor(20, true, AbstractMotor::gearset::green)});

pros::ADIDigitalOut light = pros::ADIDigitalOut('a', false);

namespace drive {
  Motor left(11, false, AbstractMotor::gearset::green);
  Motor right(12, true, AbstractMotor::gearset::green);

  void moveVelocity(double lvel, double rvel) {
    left.moveVelocity(lvel);
    right.moveVelocity(rvel);
  }

  ChassisControllerIntegrated dc = ChassisControllerFactory::create(
          left, right, AbstractMotor::gearset::green, ChassisScales({4.1_in, 15.125_in}));
  AsyncMotionProfileController dpc = AsyncControllerFactory::motionProfile(.5, 1.25, 8, dc);
} // namespace drive
