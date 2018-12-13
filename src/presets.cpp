#include "presets.h"

liftH upHeight(liftH current) {
  switch (current) {
  case lift_bottom:
    current = lift_top;
    break;
  default:
    break;
  }

  return current;
}
