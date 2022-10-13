#include "RenderSystem.h"
#include "../Components/Components.h"
#define RADIUS 5

extern Coordinator gCoordinator;

void RenderSystem::Init(SDL_Renderer* rend) {
    renderer = rend;
}

void RenderSystem::Update() {
    SDL_RenderClear(renderer);
    for(auto const& entity : entities) {
        auto render = gCoordinator.GetComponent<RenderComponent>(entity);
        auto transform = gCoordinator.GetComponent<TransformComponent>(entity);

        SDL_SetRenderDrawColor(renderer, render.r, render.g, render.b, render.a);

        for (int w = 0; w < RADIUS * 2; w++)
        {
            for (int h = 0; h < RADIUS * 2; h++)
            {
                int dx = RADIUS - w; // horizontal offset
                int dy = RADIUS - h; // vertical offset
                if ((dx*dx + dy*dy) <= (RADIUS * RADIUS))
                {
                    SDL_RenderDrawPoint(renderer, transform.position.x + dx, transform.position.y + dy);
                }
            }
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(renderer);
}

