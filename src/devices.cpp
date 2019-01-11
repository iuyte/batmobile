#include "devices.h"
#include "main.h"
#include "util.h"

#include <iostream>

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);
  /** partner joystick V5 controller */
  Controller partner(ControllerId::partner);

  const float leftDrive() {
    return master.getAnalog(ControllerAnalog::leftY) - partner.getAnalog(ControllerAnalog::rightY);
  }

  const float rightDrive() {
    return master.getAnalog(ControllerAnalog::rightY) - partner.getAnalog(ControllerAnalog::leftY);
  }

  const bool driveHoldToggle() { return master.getDigital(ControllerDigital::X); }

  const bool lift() {
    return master.getDigital(ControllerDigital::L1) - master.getDigital(ControllerDigital::L2);
  }

  const bool flipper() {
    return master.getDigital(ControllerDigital::R1) - master.getDigital(ControllerDigital::R2);
  }

  const bool intake() {
    return partner.getDigital(ControllerDigital::L1) - partner.getDigital(ControllerDigital::L2);
  }

  namespace launcher {
    const bool toggle() { return partner.getDigital(ControllerDigital::A); }

    const bool backwards() { return partner.getDigital(ControllerDigital::B); }

    const bool less() { return partner.getDigital(ControllerDigital::down); }

    const bool more() { return partner.getDigital(ControllerDigital::up); }

    const bool middleFlag() { return partner.getDigital(ControllerDigital::R2); }

    const bool highFlag() { return partner.getDigital(ControllerDigital::R1); }
  } // namespace launcher
} // namespace controller

Motor      left(11, true, AbstractMotor::gearset::green);
Motor      right(12, false, AbstractMotor::gearset::green);
Motor      intake(13, false, AbstractMotor::gearset::green);
Motor      flipper(16, true, AbstractMotor::gearset::green);
MotorGroup lift({Motor(15, false, AbstractMotor::gearset::green),
                 Motor(14, true, AbstractMotor::gearset::green)});
MotorGroup launcher({Motor(19, false, AbstractMotor::gearset::green),
                     Motor(20, true, AbstractMotor::gearset::green)});

ChassisControllerIntegrated drive = ChassisControllerFactory::create(
        left, right, AbstractMotor::gearset::green, ChassisScales({4.0625_in, 15.125_in}));
AsyncMotionProfileController dc = AsyncControllerFactory::motionProfile(.5, 1.25, 8, drive);

pros::ADIDigitalOut light = pros::ADIDigitalOut('a', false);
