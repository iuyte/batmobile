#ifndef _IUYTE_AUTONOMOUS_H_
#define _IUYTE_AUTONOMOUS_H_

#include "main.h"
using namespace okapi;

#define motorPosTargetReached(motor, range)                                                        \
  (abs(motor.getTargetPosition() - motor.getPosition()) < range)
#define motorVelTargetReached(motor, range)                                                        \
  (abs(motor.getTargetVelocity() - motor.getActualVelocity()) < range)

#define atCompetition false

typedef void (*vfptr)();

extern vfptr       auton;
extern std::string autonName;

void chooseAuton();

void autonRedFlagsCap();
void autonBlueFlagsCap();
void autonRedFlagsX2();
void autonBlueFlagsX2();

void autonRedCaps();
void autonBlueCaps();
void autonScoreCap();

void autonSkills1();
void autonSkills2();

void autonTest();

/**
 * a blocking alternative to the typical motor.moveRelative
 */
void moveToRelativePos(Motor         motor,
                       double        position,
                       double        velocity       = 200,
                       double        errorTolerance = 10,
                       unsigned long loopTime       = 10);

/**
 * a blocking alternative to the typical motor.moveAbsolute
 */
void moveToAbsolutePos(Motor         motor,
                       double        position,
                       double        velocity       = 200,
                       double        errorTolerance = 10,
                       unsigned long loopTime       = 10);

#endif
