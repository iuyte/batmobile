#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "main.h"

#define MAGIC 2

// catapult position
static const float cataPos[] = {1450, 2000};

// catapult states
enum CataState{
        neutral,
        ready,
        fire,
};

CataState toggleState(CataState newState = CataState::neutral);
void catapultTask(void *none);
extern pros::Task *catapultTaskHandle;

void infoLoop(void *none);
void chooseAuton();

#endif
