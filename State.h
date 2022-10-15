#ifndef PHYSICSSIM_STATE_H
#define PHYSICSSIM_STATE_H
#include <unordered_map>
#include "ECS.h"
#include "Vec2.h"

class State {
public:
    std::unordered_map<Entity, Vec2d> positions {};
};


#endif //PHYSICSSIM_STATE_H
