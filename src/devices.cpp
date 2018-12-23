#include "devices.h"
#include "main.h"
#include "util.h"

#include <iostream>

Controller controller;
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
