#include "Engine.h"

int main(int argv, char* argc[])
{
    Engine engine;

    if (engine.Init())
    {
        engine.Run();
    }
    engine.Shutdown();

    return 0;
}