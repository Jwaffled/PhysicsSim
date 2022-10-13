#ifndef PHYSICSSIM_PHYSICALCOMPONENT_H
#define PHYSICSSIM_PHYSICALCOMPONENT_H
#include "../Vec2.h"

class PhysicalComponent {
public:
    float mass{};
    Vec2d activeForce;
    PhysicalComponent() = default;
    PhysicalComponent& WithMass(float m) {
        mass = m;
        return *this;
    }
    PhysicalComponent& WithForce(double x, double y) {
        activeForce = Vec2d(x, y);
        return *this;
    }
};

#endif //PHYSICSSIM_PHYSICALCOMPONENT_H
