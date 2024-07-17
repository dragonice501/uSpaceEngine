#include "Engine.h"
#include "GraphicsConstants.h"

#include "OrbitScene.h"
#include "RandomSystemScene.h"
#include "RocketScene.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_sdlrenderer2.h"

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

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(Graphics::GetWindow(), Graphics::GetRenderer());
    ImGui_ImplSDLRenderer2_Init(Graphics::GetRenderer());

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

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        scene.Input();
        scene.Update(deltaTime);
        scene.Render();

        //Graphics::FlipScreen();
        Graphics::PresentFrame();
    }

    scene.Destroy();
}

void Engine::Shutdown()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    Graphics::CloseWindow();
}