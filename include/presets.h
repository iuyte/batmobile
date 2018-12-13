#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "devices.h"
#include "main.h"
#include "util.h"

/** lift heights */
typedef enum {
  lift_bottom = 0,
  lift_top    = 360 * 30,
} liftH;

/** raise the lift by one level */
liftH upHeight(liftH current);

#endif
