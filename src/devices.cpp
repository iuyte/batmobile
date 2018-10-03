#include "devices.h"
#include "main.h"
#include "util.h"

#include <iostream>

okapi::Controller controller;
okapi::MotorGroup
    left({okapi::Motor(1, false, okapi::AbstractMotor::gearset::green),
          okapi::Motor(3, false, okapi::AbstractMotor::gearset::green)});
okapi::MotorGroup
    right({okapi::Motor(2, true, okapi::AbstractMotor::gearset::green),
           okapi::Motor(4, true, okapi::AbstractMotor::gearset::green)});
