#include "Engine.h"

int main(int argv, char* argc[])
{
    Engine app;
    app.Setup();

    while (app.IsRunning())
    {
        app.Input();
        app.Update();
        app.Render();
    }

    app.Destroy();

    return 0;
}