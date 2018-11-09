#include "devices.h"
#include "main.h"
#include "util.h"

#include <iostream>
using namespace okapi;

Controller controller;
Motor      left(13, false, okapi::AbstractMotor::gearset::green);
Motor      right(12, true, okapi::AbstractMotor::gearset::green);
Motor      launcher(11, false, okapi::AbstractMotor::gearset::green);
Motor      intake(20, false, okapi::AbstractMotor::gearset::green);
MotorGroup lift({Motor(9, true, AbstractMotor::gearset::green),
                 Motor(10, false, AbstractMotor::gearset::green)});

AsyncPosIntegratedController launchC = okapi::AsyncControllerFactory::posIntegrated(launcher, 200);
ChassisControllerIntegrated  drive =
        okapi::ChassisControllerFactory::create(left,
                                                right,
                                                okapi::AbstractMotor::gearset::green,
                                                okapi::ChassisScales({4.1875_in, 10.5_in}));
AsyncMotionProfileController dc = okapi::AsyncControllerFactory::motionProfile(.5, 1.25, 8, drive);

Pot pot('h');

void Pot::reset() { this->z = this->get(); }

void   Pot::setScale(double scalv) { scale = scalv; }
double Pot::get() { return (get_value() - this->z) * scale; }
