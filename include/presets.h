#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "main.h"

#define MAGIC 2

// low/high flag flywheel speeds, respectively
static const float fpreset[] = {110, 125, 130};

// catapult position
static const float cataPos[] = {1480, 2000};

// catapult states
enum CataState{
        neutral,
        ready,
        fire,
};

CataState toggleState();
void catapultTask(void *none);

void infoLoop(void *none);
void chooseAuton();

#endif
