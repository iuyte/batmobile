#include "devices.h"

namespace controller {
  using okapi::ControllerAnalog;
  using okapi::ControllerDigital;

  /** main V5 controller */
  Controller master(ControllerId::master);
  Controller partner(ControllerId::partner);

  namespace get {
    namespace drive {
      const float forward() {
        return master.getAnalog(ControllerAnalog::leftY) -
               partner.getAnalog(ControllerAnalog::leftY);
      }

      const float turn() {
        return master.getAnalog(ControllerAnalog::rightX) +
               partner.getAnalog(ControllerAnalog::rightX);
      }

      const float strafe() {
        return master.getAnalog(ControllerAnalog::leftX) -
               partner.getAnalog(ControllerAnalog::leftX);
      }
      const bool holdToggle() { return master.getDigital(ControllerDigital::X); }
    } // namespace drive

    const float intake() {
      return master.getDigital(ControllerDigital::R1) - master.getDigital(ControllerDigital::R2);
    }

    const int catapult() {
      static unsigned long lastTime = millis();

      if (millis() - lastTime > 350) {
        if (controller::master.getDigital(ControllerDigital::L1)) {
          lastTime = millis();
          return 1;
        } else if (controller::master.getDigital(ControllerDigital::L2)) {
          lastTime = millis();
          return -1;
        }
      }

      return 0;
    }
  } // namespace get
} // namespace controller

Motor intake(12, true, AbstractMotor::gearset::green);

namespace catapult {
  MotorGroup    motor({Motor(21, false, AbstractMotor::gearset::red),
                    Motor(20, true, AbstractMotor::gearset::red)});
  Potentiometer pot('a');
} // namespace catapult

namespace drive {
  Motor      motors[2][2]{{Motor(16, false, AbstractMotor::gearset::green),
                      Motor(9, false, AbstractMotor::gearset::green)},
                     {Motor(15, true, AbstractMotor::gearset::green),
                      Motor(10, true, AbstractMotor::gearset::green)}};
  MotorGroup left({motors[0][0], motors[0][1]});
  MotorGroup right({motors[1][0], motors[1][1]});

  void moveVelocity(double lvel, double rvel) {
    left.moveVelocity(lvel);
    right.moveVelocity(rvel);
  }

  void control(float forward, float turn, float strafe) {
#define dmax 200 * 1.5
    motors[0][0].moveVelocity(trim(dmax * (forward + turn + strafe), -dmax, dmax));
    motors[0][1].moveVelocity(trim(dmax * (forward + turn - strafe), -dmax, dmax));
    motors[1][0].moveVelocity(trim(dmax * (forward - turn - strafe), -dmax, dmax));
    motors[1][1].moveVelocity(trim(dmax * (forward - turn + strafe), -dmax, dmax));
  }

  using G = IterativePosPIDController::Gains;
  shared_ptr<ChassisController> dc =
          ChassisControllerBuilder()
                  .withMotors(left, right)
                  .withGearset(AbstractMotor::gearset::green)
                  .withDimensions(ChassisScales({4_in, 14_in}, imev5GreenTPR))
                  // .withGains(G{.0031, 0, .00019, 0}, G{0, 0, 0, 0}, G{0, 0, 0, 0})
                  .build();
  shared_ptr<AsyncMotionProfileController> dpc = AsyncMotionProfileControllerBuilder()
                                                         .withOutput(dc)
                                                         .withLimits(PathfinderLimits({1, 2, 10}))
                                                         .buildMotionProfileController();
  bool atTarget(int range) {
    for (auto &&i = 0; i < 2; i++)
      for (auto &&ii = 0; i < 2; i++)
        if (!motorPosTargetReached(motors[i][ii], range))
          return false;

    return true;
  }

  double totalVelocity() {
    double tv;

    for (auto &&i = 0; i < 2; i++)
      for (auto &&ii = 0; i < 2; i++)
        tv += abs(motors[i][ii].get_actual_velocity());

    return tv;
  }

  double backVelocity() {
    double tv;

    for (auto &&i = 0; i < 2; i++)
      tv += abs(motors[i][0].get_actual_velocity());

    return tv;
  }

  ADIGyro gyro('b', .104);

  namespace _g {
    double        p;
    double        g1;
    double        g2;
    bool          gb  = true;
    bool          sty = false;
    unsigned long st;
  } // namespace _g

  double getAngle() {
    using namespace _g;
    if (!sty) {
      st  = millis();
      sty = true;
    }

    return gyro.get(); // + .000012 * (millis() - st);

    double tv = totalVelocity();

    if (abs(tv) < 1.f) {
      gb = true;
      return p;
    }

    return (p = (p + gyro.get() + .0000125 * (millis() - st)) / 2);

    if (gb) {
      g2 += (p - g1);
      g1 = p;
      gb = true;
      p -= g2;
    }

    return p;
  }

  void turn(float angle, float range, bool absolute) {
    angle = (angle * 55.8f) / 9.f;
    moveRelative(angle, -angle, 115);
    waitUntilCompletion();

    // const float kp     = 2.1;
    // const float min    = 45;
    // float       target = absolute ? angle : angle + getAngle();
    // float       error;

    // do {
    //   error = target - getAngle();
    //   moveVelocity(cutRange(kp * error, -min, min), cutRange(-kp * error, -min, min));

    //   delay(10);
    // } while (abs(error) > range);

    moveVelocity(0, 0);
  }

  void strafe(float ticks, int vel) {
    ticks = dt(ticks);
    motors[0][0].moveRelative(ticks, vel);
    motors[0][1].moveRelative(-ticks, vel);
    motors[1][0].moveRelative(-ticks, vel);
    motors[1][1].moveRelative(ticks, vel);
  }

  void moveRelative(float l, float r, int vel) {
    left.moveRelative(dt(l), vel);
    right.moveRelative(dt(r), vel);
  }

  void moveAbsolute(float l, float r, int vel) {
    left.moveAbsolute(dt(l), vel);
    right.moveAbsolute(dt(r), vel);
  }

  void waitUntilCompletion(unsigned long minTime) {
    waitUntil(totalVelocity() > 10, 20);
    delay(minTime);
    waitUntil(totalVelocity() < 3, 20);
  }

  void waitUntilStarted() { waitUntil(totalVelocity() > 10, 20); }

  void waitUntilStopped() { waitUntil(backVelocity() < 4, 20); }

  void reset() {
    gyro.reset();
    drive::dc->stop();

    for (auto &&i = 0; i < 2; i++)
      for (auto &&ii = 0; i < 2; i++)
        motors[i][ii].getEncoder()->reset();

    _g::g1  = 0;
    _g::g2  = 0;
    _g::p   = 0;
    _g::gb  = true;
    _g::sty = false;
  }
} // namespace drive

pros::Vision vision(7, pros::E_VISION_ZERO_CENTER);
