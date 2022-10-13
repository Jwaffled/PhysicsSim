#ifndef PHYSICSSIM_RENDERSYSTEM_H
#define PHYSICSSIM_RENDERSYSTEM_H
#include "../ECS.h"
#include <SDL.h>

class RenderSystem : public System {
private:
    SDL_Renderer* renderer;
public:
    void Init(SDL_Renderer* rend);
    void Update();
};

#undef RADIUS
#endif //PHYSICSSIM_RENDERSYSTEM_H
