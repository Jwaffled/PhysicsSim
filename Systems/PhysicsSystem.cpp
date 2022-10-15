#include "PhysicsSystem.h"
#include "../Components/Components.h"

extern Coordinator gCoordinator;

void PhysicsSystem::Update(State& state, double dt) {
    for(const auto& entity : entities) {
        auto& transform = gCoordinator.GetComponent<TransformComponent>(entity);
        auto& physical = gCoordinator.GetComponent<PhysicalComponent>(entity);
        auto& motion = gCoordinator.GetComponent<MotionComponent>(entity);

        motion.accel = physical.activeForce / physical.mass;
        physical.activeForce = Vec2d();
        motion.vel += motion.accel * dt;
        transform.position += motion.vel * dt;
        state.positions[entity] = transform.position;
    }
}