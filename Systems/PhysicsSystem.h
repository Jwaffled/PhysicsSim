#ifndef PHYSICSSIM_PHYSICSSYSTEM_H
#define PHYSICSSIM_PHYSICSSYSTEM_H
#include "../ECS.h"
#include "../State.h"

class PhysicsSystem : public System {
public:
    void Update(State& state, double dt);
};


#endif //PHYSICSSIM_PHYSICSSYSTEM_H
