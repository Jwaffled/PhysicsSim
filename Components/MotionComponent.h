#ifndef PHYSICSSIM_MOTIONCOMPONENT_H
#define PHYSICSSIM_MOTIONCOMPONENT_H
#include "../Vec2.h"

class MotionComponent {
public:
    Vec2d vel;
    Vec2d accel;
    MotionComponent() = default;
    MotionComponent& WithVelocity(double x, double y) {
        vel = Vec2d(x, y);
        return *this;
    }
    MotionComponent& WithAccel(double x, double y) {
        accel = Vec2d(x, y);
        return *this;
    }
};

#endif //PHYSICSSIM_MOTIONCOMPONENT_H
