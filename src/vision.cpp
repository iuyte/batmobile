#include "vision.h"
#include "main.h"

namespace vision {
  void vision::alignX(Alignment a, unsigned long hold, float offset, float prec) {
    unsigned long           unreached;
    pros::vision_object_s_t rtn;
    float                   m;

    do {
      m = get_by_sig(0, a).x_middle_coord + offset;
      drive::control(0, 0, .0065 * m);
      delay(50);
      if (abs(m) > prec)
        unreached = millis();
    } while (millis() - unreached < hold);

    drive::moveVelocity(0, 0);
  }

  void vision::alignY(Alignment a, float objheight, unsigned long hold, float prec) {
    unsigned long           unreached;
    pros::vision_object_s_t rtn;
    float                   m;

    do {
      m = objheight - rtn.height;
      drive::moveVelocity(sgn(m) * 200, sgn(m) * 200);
      rtn = get_by_sig(0, a);
      delay(50);
      if (abs(m) > prec)
        unreached = millis();
    } while (millis() - unreached < hold);

    drive::moveVelocity(0, 0);
  }
} // namespace vision
