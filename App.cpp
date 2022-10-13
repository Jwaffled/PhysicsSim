#include "App.h"
#include "Components/Components.h"
#include <iostream>
#include <chrono>

Coordinator gCoordinator;

App& App::WithFPS(const int frames) {
    fps = frames;
    return *this;
}

App& App::WithResolution(const int x, const int y) {
    width = x;
    height = y;
    return *this;
}

/* TODO
 *  Fix timestep
 *  Add collisions based on conservation of energy/momentum
 *      - Sweep and prune
 *      - Maybe bounding volume hierarchy
 *  Create "smoother" circles when rendering (anti-aliasing? whatever it's called)
 *  RUN PHYSICS AT A FIXED RATE, THEN LERP BETWEEN FRAMES
 */

bool App::InitSDL() {
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        success = false;
        std::cerr << "Something went wrong initializing SDL. SDL ERROR: " << SDL_GetError() << '\n';
    } else {
        window = SDL_CreateWindow("Physics Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if(window == nullptr) {
            std::cerr << "Window init failed. SDL ERROR: " << SDL_GetError() << '\n';
            success = false;
        } else {
            screenSurface = SDL_GetWindowSurface(window);
            renderer = SDL_CreateRenderer(window, -1, 0);
        }
    }

    return success;
}

void App::Init() {
    gCoordinator.Init();

    RegisterComponents();

    InitSystems();
}

void App::InitSystems() {
    gravitationalSystem = gCoordinator.RegisterSystem<GravitationalSystem>();
    {
        Signature sig;
        sig.set(gCoordinator.GetComponentType<TransformComponent>());
        sig.set(gCoordinator.GetComponentType<PhysicalComponent>());
        gCoordinator.SetSystemSignature<GravitationalSystem>(sig);
    }

    physicsMotionSystem = gCoordinator.RegisterSystem<PhysicsSystem>();
    {
        Signature sig;
        sig.set(gCoordinator.GetComponentType<TransformComponent>());
        sig.set(gCoordinator.GetComponentType<MotionComponent>());
        sig.set(gCoordinator.GetComponentType<PhysicalComponent>());
        gCoordinator.SetSystemSignature<PhysicsSystem>(sig);
    }

    renderSystem = gCoordinator.RegisterSystem<RenderSystem>();
    {
        Signature sig;
        sig.set(gCoordinator.GetComponentType<TransformComponent>());
        sig.set(gCoordinator.GetComponentType<RenderComponent>());
        gCoordinator.SetSystemSignature<RenderSystem>(sig);
    }
    renderSystem->Init(renderer);
}

void App::Run() {
    auto entity1 = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent<PhysicalComponent>(entity1,
                                                     PhysicalComponent()
                                                     .WithMass(5e11));
        gCoordinator.AddComponent<TransformComponent>(entity1,
                                                      TransformComponent()
                                                      .WithPosition(100, 100));
        gCoordinator.AddComponent<MotionComponent>(entity1,
                                                   MotionComponent()
                                                   .WithVelocity(50, 0));
        gCoordinator.AddComponent<RenderComponent>(entity1,
                                                   RenderComponent()
                                                   .WithColor(255, 0, 0, 255));
    }
    auto entity2 = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent<PhysicalComponent>(entity2,
                                                     PhysicalComponent()
                                                     .WithMass(1e17));
        gCoordinator.AddComponent<TransformComponent>(entity2,
                                                      TransformComponent()
                                                      .WithPosition(500, 170));
        gCoordinator.AddComponent<MotionComponent>(entity2,
                                                   MotionComponent()
                                                   .WithVelocity(-20, 20));
        gCoordinator.AddComponent<RenderComponent>(entity2,
                                                   RenderComponent()
                                                   .WithColor(255, 255, 0, 255));
    }

    // Main loop here
    SDL_Event e;
    bool quit = false;

    const double dt = 1.0 / fps;

    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if( e.type == SDL_QUIT ) quit = true;
        }
        gravitationalSystem->Update();


        physicsMotionSystem->Update(dt);
        renderSystem->Update();
        SDL_Delay(dt * 1000);
    }

    Destroy();
}

void App::Destroy() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(screenSurface);
    window = nullptr;
    renderer = nullptr;
    screenSurface = nullptr;

    SDL_Quit();
}