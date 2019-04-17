#include "devices.h"
#include "main.h"

namespace vision {
  void alignX(Alignment a, float offset, float prec) {
    pros::vision_object_s_t rtn;
    float                   m;

    do {
      m = vision.get_by_sig(0, a).x_middle_coord + offset;
      drive::control(0, 0, .0065 * m);
      delay(50);
    } while (abs(m) > prec);

    drive::moveVelocity(0, 0);
  }

  void alignY(Alignment a, float objheight, float prec) {
    pros::vision_object_s_t rtn;
    float                   m;

    do {
      m = objheight - rtn.height;
      drive::moveVelocity(sgn(m) * 200, sgn(m) * 200);
      rtn = vision.get_by_sig(0, a);
      delay(50);
    } while (abs(m) > prec);

    drive::moveVelocity(0, 0);
  }
} // namespace vision
