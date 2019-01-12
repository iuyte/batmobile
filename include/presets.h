#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "main.h"

// low/high flag flywheel speeds, respectively
static const float fpreset[2] = {99, 126};

// flipper speed in rpm
static const int flpSpeed = 140;
// bottom flipper height
static const int flpMin   = 0;
// top flipper height
static const int flpMax   = flpMin + 500;
// flipper height for holding a cap in normal position
static const int flpHold  = flpMin + 70;

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

#endif
