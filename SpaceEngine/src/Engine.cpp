#include "Engine.h"
#include "GraphicsConstants.h"

#include "OrbitScene.h"
#include "RandomSystemScene.h"
#include "RocketScene.h"

#include <SDL.h>

bool Engine::isRunning = false;
int Engine::timeAccelIndex = 1;
int Engine::timeAccelerationSpeeds[10] = { 0, 1, 2, 3, 4, 5, 10, 100, 1000, 10000 };

bool Engine::IsRunning()
{
    return isRunning;
}

bool Engine::Init()
{
    if (!Graphics::OpenWindow()) return false;

    isRunning = true;

    return true;
}

void Engine::Run()
{
    OrbitScene scene;
    //RocketScene scene;
    
    scene.Setup();

    while (isRunning)
    {
        static int timePreviousFrame;
        int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
        if (timeToWait > 0)
        {
            SDL_Delay(timeToWait);
        }

        deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
        if (deltaTime > 0.016f) deltaTime = 0.016f;
        deltaTime *= timeAccelerationSpeeds[timeAccelIndex];

        timePreviousFrame = SDL_GetTicks();
        
        Graphics::ClearScreen(0xFF111122);

        scene.Input();
        scene.Update(deltaTime);
        scene.Render();

        Graphics::FlipScreen();
        Graphics::PresentFrame();
    }

    scene.Destroy();
}

void Engine::Shutdown()
{
    Graphics::CloseWindow();
}