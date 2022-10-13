#ifndef PHYSICSSIM_TRANSFORMCOMPONENT_H
#define PHYSICSSIM_TRANSFORMCOMPONENT_H

#include "../Vec2.h"

class TransformComponent {
public:
    Vec2d position;
    TransformComponent() = default;
    TransformComponent& WithPosition(double x, double y) {
        position = Vec2d(x, y);
        return *this;
    }
};

#endif //PHYSICSSIM_TRANSFORMCOMPONENT_H
