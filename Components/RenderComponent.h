#ifndef PHYSICSSIM_RENDERCOMPONENT_H
#define PHYSICSSIM_RENDERCOMPONENT_H


#include <SDL.h>

class RenderComponent {
public:
    Uint8 r {}, g {}, b {}, a {};
    RenderComponent() = default;
    RenderComponent& WithColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
        r = red;
        g = green;
        b = blue;
        a = alpha;

        return *this;
    }
};


#endif //PHYSICSSIM_RENDERCOMPONENT_H
