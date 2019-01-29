#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "main.h"

#define MAGIC 2

// low/high flag flywheel speeds, respectively
static const float fpreset[] = {110, 125, 130};

// lift speed in rpm
static const int liftSpeed = 185;

/** lift heights */
typedef enum {
  lift_bottom = 0,
  lift_ing    = 300,
  lift_top    = 1780,
} liftH;

/** raise the lift by one level */
liftH upHeight(liftH current);

void infoLoop(void *none);
void chooseAuton();

#endif
