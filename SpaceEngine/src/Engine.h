#pragma once

#include "Graphics.h"
#include "Body.h"

#include "Ship.h"
#include "Satellite.h"

#include <vector>

class Engine
{
private:
    static bool isRunning;
    static int timeAccelIndex;
    static int timeAccelerationSpeeds[10];

    float deltaTime = 0;

public:
    Engine() = default;
    ~Engine() = default;

    static void SetIsRunning(bool running) { isRunning = running; }
    static void IncrementTimeAccel() { timeAccelIndex++; if (timeAccelIndex > 9) timeAccelIndex = 9; }
    static void DecrementTimeAccel() { timeAccelIndex--; if (timeAccelIndex < 0) timeAccelIndex = 0; }
    static int GetTimeAccel() { return timeAccelerationSpeeds[timeAccelIndex]; }

    bool Init();
    void Run();
    void Shutdown();

    bool IsRunning();
};