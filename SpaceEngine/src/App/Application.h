#pragma once

#include "Graphics.h"
#include "Body.h"

#include "../Game/Ship.h"
#include "../Game/Satellite.h"

#include <vector>

class Application
{
    private:
        bool running = false;
        float deltaTime = 0;

        int timeAccelIndex = 1;
        int timeAccelerationSpeeds[6] = { 0, 1, 2, 3, 5, 10 };

        Ship* ship = nullptr;
        Satellite* tulli = nullptr;
        Satellite* yobo = nullptr;

        std::vector<Satellite*> satellites;
        std::vector<Satellite*> influencingSatellites;
        Satellite* nearestSatellite = nullptr;

        bool mouseButtonDown = false;
        Vec2 screenOffset = { 0.0f, 0.0f };

        bool showVectors = true;
        bool showTrajectory = true;
        bool showTutorial = true;

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
        
        void RenderTutorialText();
        void RenderUI();
};