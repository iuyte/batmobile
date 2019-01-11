#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "devices.h"
#include "main.h"
#include "util.h"
#include "switcher.h"

// flipper speed in rpm
static const int flpSpeed = 140;
// bottom flipper height
static const int flpMin   = 0;
// top flipper height
static const int flpMax   = flpMin + 500;
// flipper height for holding a cap in normal position
static const int flpHold  = flpMin + 70;

/** lift heights */
typedef enum {
  lift_bottom = 0,
  lift_ing    = 300,
  lift_top    = 1820,
} liftH;

/** raise the lift by one level */
liftH upHeight(liftH current);

#endif
