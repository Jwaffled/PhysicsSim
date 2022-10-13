#include "App.h"

int main(int argc, char* argv[]) {
    App app = App()
            .WithFPS(240)
            .WithResolution(1200, 800);

    if(!app.InitSDL()) {
        return -1;
    }

    app.Init();
    app.Run();

    return 0;
}
