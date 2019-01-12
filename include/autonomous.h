#ifndef _IUYTE_AUTONOMOUS_H_
#define _IUYTE_AUTONOMOUS_H_

#include "main.h"

typedef void (*vfptr)();

extern vfptr auton;

void chooseAuton();

void printData();
void autonSkills();
void autonRedFlags();
void autonBlueFlags();
void autonDriveStraight();

#endif
