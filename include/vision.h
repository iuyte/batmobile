#ifndef _IUYTE_VISION_H_
#define _IUYTE_VISION_H_

#include "devices.h"
#include "main.h"

namespace vision {
  enum Alignment {
    Flag     = 2,
    Platform = 1,
  };

  class vision : public pros::Vision {
  public:
    void alignX(Alignment a, unsigned long hold = 200, float offset = 0, float prec = 6);
    void alignY(Alignment a, float objheight, unsigned long hold = 200, float prec = 2);

    using pros::Vision::Vision;
  };

  /** front vision sensor */
  extern vision front;
  /** back vision sensor */
  extern vision back;
} // namespace vision

#endif
