#pragma once

#include "Scene.h"

#include "Graphics.h"
#include "Body.h"

#include "Ship.h"
#include "Satellite.h"

#include <vector>

class RocketScene : public Scene
{
private:
    float deltaTime = 0;

    int timeAccelIndex = 1;
    int timeAccelerationSpeeds[6] = { 0, 1, 2, 3, 5, 10 };

    Ship* ship = nullptr;
    Satellite* tulli = nullptr;
    Satellite* yobo = nullptr;

    std::vector<Satellite*> satellites;
    std::vector<Satellite*> influencingSatellites;

    bool mouseButtonDown = false;
    Vec2 screenOffset = { 0.0f, 0.0f };

    bool showTutorial = true;

public:
    RocketScene() = default;
    ~RocketScene() = default;

    void Setup() override;
    void Destroy() override;

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;

    void Reset();

    void RenderTutorialText();
    void RenderUI();
};