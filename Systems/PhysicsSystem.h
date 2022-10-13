#ifndef PHYSICSSIM_PHYSICSSYSTEM_H
#define PHYSICSSIM_PHYSICSSYSTEM_H
#include "../ECS.h"

class PhysicsSystem : public System {
public:
    void Update(double dt);
};


#endif //PHYSICSSIM_PHYSICSSYSTEM_H
