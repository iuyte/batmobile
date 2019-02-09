#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "main.h"

#define MAGIC 2

// catapult position
static const float cataPos[] = {1550, 2000, 2350};

// catapult states
enum CataState{
        neutral,
        ready,
        fire,
};

extern pros::Task *catapultTaskHandle;
CataState toggleState(CataState newState = CataState::neutral);
void catapultTask(void *none);
bool catapultAchieved();

void infoLoop(void *none);
void chooseAuton();

#endif
