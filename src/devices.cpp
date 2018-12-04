#include "devices.h"
#include "main.h"
#include "util.h"

#include <iostream>

Controller controller;
Motor      left(12, false, AbstractMotor::gearset::green);
Motor      right(13, true, AbstractMotor::gearset::green);
Motor      intake(10, false, AbstractMotor::gearset::green);
Motor      flipper(8, true, AbstractMotor::gearset::green);
MotorGroup lift({Motor(1, true, AbstractMotor::gearset::green),
                 Motor(2, false, AbstractMotor::gearset::green)});
MotorGroup launcher({Motor(19, false, AbstractMotor::gearset::green),
                     Motor(20, true, AbstractMotor::gearset::green)});

ChassisControllerIntegrated drive = ChassisControllerFactory::create(
        left, right, AbstractMotor::gearset::green, ChassisScales({4.1875_in, 10.5_in}));
AsyncMotionProfileController dc = AsyncControllerFactory::motionProfile(.5, 1.25, 8, drive);

::Pot pot('h');

int32_t Pot::get() { return (get_value() - zero) * scale; }
