#ifndef _IUYTE_PRESETS_H_
#define _IUYTE_PRESETS_H_

#include "devices.h"
#include "main.h"
#include "util.h"

/** ready the launcher by bringing the arm back all the way */
void launcherReady();
/** move the launcher a bit farther, causing the slip-gear to allow the rubber bands to swing the
 * catapult arm quite forcfully */
void launcherFire();

/** lift heights */
typedef enum {
  lift_bottom = 0,
  lift_top    = 360 * 30,
} liftH;

/** raise the lift by one level */
liftH upHeight(liftH current);

#endif
