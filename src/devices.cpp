#include "devices.h"
#include "main.h"
#include "util.h"

#include <iostream>
using namespace okapi;

Controller controller;
Motor      left(13, false, okapi::AbstractMotor::gearset::green);
Motor      right(12, true, okapi::AbstractMotor::gearset::green);
Motor      launcher(11, true, okapi::AbstractMotor::gearset::green);
Motor      intake(20, false, okapi::AbstractMotor::gearset::green);

ChassisControllerIntegrated drive = okapi::ChassisControllerFactory::create(
    left, right, okapi::AbstractMotor::gearset::green, okapi::ChassisScales({4.1875, 10.5}));
