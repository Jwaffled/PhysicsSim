#ifndef PHYSICSSIM_RENDERSYSTEM_H
#define PHYSICSSIM_RENDERSYSTEM_H
#include "../ECS.h"
#include "../Components/TransformComponent.h"
#include "../State.h"
#include <SDL.h>

class RenderSystem : public System {
private:
    SDL_Renderer* renderer;
    std::unordered_map<Entity, Vec2d> previousState {};
    std::unordered_map<Entity, Vec2d> currentState {};
public:
    void Init(SDL_Renderer* rend);
    void Update(const State& state, bool captureNewState, double alpha);
};

#undef RADIUS
#endif //PHYSICSSIM_RENDERSYSTEM_H
