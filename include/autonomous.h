#ifndef _IUYTE_AUTONOMOUS_H_
#define _IUYTE_AUTONOMOUS_H_

#include "main.h"

#define motorPosTargetReached(motor, range)                                                        \
  (abs(motor.getTargetPosition() - motor.getPosition()) < range)
#define motorVelTargetReached(motor, range)                                                        \
  (abs(motor.getTargetVelocity() - motor.getActualVelocity()) < range)

typedef void (*vfptr)();

extern vfptr       auton;
extern std::string autonName;

void chooseAuton();

void printData();
void autonSkills();
void autonRedFlags();
void autonBlueFlags();
void autonRedCaps();
void autonBlueCaps();
void autonScoreCap();

#endif
