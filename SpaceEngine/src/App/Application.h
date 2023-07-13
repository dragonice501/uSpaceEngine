#pragma once

#include "Graphics.h"
#include "Body.h"

#include <vector>

class Application
{
    private:
        bool running = false;
        float deltaTime = 0;

        int timeAccelIndex = 1;
        int timeAccelerationSpeeds[6] = { 0, 1, 2, 3, 5, 10 };
        
        Body* ship = nullptr;
        Body* moon = nullptr;

        bool mouseButtonDown = false;
        Vec2 screenOffset = { 0.0f, 0.0f };

        bool showVectors = true;
        bool showTrajectory = true;
        bool showTutorial = true;

        bool throttleForwardPressed = false;
        bool throttleBackPressed = false;
        bool turnRightPressed = false;
        bool turnLeftPressed = false;

        float throttle = 0.0f;
        float throttleMax = 1.0f;
        float throttleRate = 0.5f;
        float thrust = 1.0f;
        Vec2 thrustVector = { 0.0f, -1.0f };

        float turnSpeed = 1.0f;

        float fuel = 1.0f;
        float fuelMaxBurnRate = 0.01f;

    public:
        Application() = default;
        ~Application() = default;

        void Setup();
        void Destroy();

        bool IsRunning();
        
        void Input();
        void Update();
        void Render();

        void Reset();

        void UpdateShip(float dt);
        
        void RenderTutorialText();
        void RenderUI();
};