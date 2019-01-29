#include "devices.h"

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);
  /** partner joystick V5 controller */
  Controller partner(ControllerId::partner);

  const float leftDrive() {
    return partner.getAnalog(ControllerAnalog::leftY) - master.getAnalog(ControllerAnalog::rightY);
  }

  const float rightDrive() {
    return partner.getAnalog(ControllerAnalog::rightY) - master.getAnalog(ControllerAnalog::leftY);
  }

  const bool driveHoldToggle() { return partner.getDigital(ControllerDigital::X); }

  const float lift() {
    return partner.getDigital(ControllerDigital::L1) - partner.getDigital(ControllerDigital::L2);
  }

  const float intake() {
    return master.getDigital(ControllerDigital::R1) - master.getDigital(ControllerDigital::R2);
  }

  namespace launcher {
    const bool off() { return master.getDigital(ControllerDigital::A); }

    const bool backwards() { return master.getDigital(ControllerDigital::B); }

    const bool less() { return master.getDigital(ControllerDigital::down); }

    const bool more() { return master.getDigital(ControllerDigital::up); }

    const bool middleFlag() { return master.getDigital(ControllerDigital::L2); }

    const bool highFlag() { return master.getDigital(ControllerDigital::L1); }
  } // namespace launcher
} // namespace controller

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

  ChassisControllerIntegrated dc = ChassisControllerFactory::create(
          left, right, AbstractMotor::gearset::green, ChassisScales({4.0625_in, 15.125_in}));
  AsyncMotionProfileController dpc = AsyncControllerFactory::motionProfile(.5, 1.25, 8, dc);
} // namespace drive
