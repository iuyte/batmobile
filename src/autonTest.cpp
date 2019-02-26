#include "devices.h"
#include "autonomous.h"
#include "util.h"

void autonTest() {
  drive::dc->moveDistanceAsync(24_in);
  drive::dc->waitUntilSettled();
  delay(1000);
  drive::dc->moveDistanceAsync(-24_in);
  drive::dc->waitUntilSettled();
}
