#include "Components.h"
#include "../ECS.h"

extern Coordinator gCoordinator;

void RegisterComponents() {
    gCoordinator.RegisterComponent<TransformComponent>();
    gCoordinator.RegisterComponent<PhysicalComponent>();
    gCoordinator.RegisterComponent<MotionComponent>();
    gCoordinator.RegisterComponent<RenderComponent>();
}