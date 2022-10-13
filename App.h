#ifndef PHYSICSSIM_APP_H
#define PHYSICSSIM_APP_H

#include <SDL.h>
#include "Systems/Systems.h"

class App {
private:
    int fps{60};
    int width{800};
    int height{600};
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Surface* screenSurface;
    std::shared_ptr<GravitationalSystem> gravitationalSystem;
    std::shared_ptr<PhysicsSystem> physicsMotionSystem;
    std::shared_ptr<RenderSystem> renderSystem;
    void Destroy();
    void InitSystems();
public:
    App() = default;
    App& WithFPS(int fps);
    App& WithResolution(int width, int height);
    bool InitSDL();
    void Init();
    void Run();
};

#endif //PHYSICSSIM_APP_H
