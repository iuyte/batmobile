#include "vision.h"
#include "main.h"

namespace vision {
  void vision::alignX(Alignment a, unsigned long hold, float offset, float prec) {
    unsigned long           unreached;
    pros::vision_object_s_t rtn;
    float                   m;

    do {
      m = get_by_sig(0, a).x_middle_coord + offset;
      drive::control(0, 0, trim(.0077 * m, -.85, .85));
      delay(50);
      if (abs(m) > prec)
        unreached = millis();
    } while (millis() - unreached < hold);

    drive::moveVelocity(0, 0);
  }

  void vision::alignY(Alignment a, float objheight, unsigned long hold, float prec) {
    pros::vision_object_s_t rtn = get_by_sig(0, a);
    float                   m   = objheight - rtn.height;
    int                     s   = sgn(objheight - rtn.height);

    do {
      drive::moveVelocity(sgn(m) * -200, sgn(m) * -200);
      delay(50);
      rtn = get_by_sig(0, a);
      m   = objheight - rtn.height;
    } while (sgn(m) == sgn(s));
  }
} // namespace vision
