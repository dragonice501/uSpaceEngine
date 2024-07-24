#pragma once

#include "Scene.h"

#include "CelestialBody.h"
#include "Rocket.h"

#include <vector>

class RocketScene : public Scene
{
private:
    bool mouseButtonDown = false;

    CelestialBody planet;
    Rocket rocket;

    std::vector<Vec3Double> points;

public:
    RocketScene() = default;
    ~RocketScene() = default;

    void Setup() override;
    void Destroy() override;

    void Input() override;
    void Update(float deltaTime) override;
    void Render() override;

    void GeneratePoints(float start, float end, int count);
    Vec3Double GeneratePoint(float p);
    void Tessellate(Vec3Double a, Vec3Double b, int t);
    float ComputeTriangleArea(const Vec3Double& a, const Vec3Double& b, const Vec3Double& c);
};