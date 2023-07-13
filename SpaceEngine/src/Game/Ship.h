#pragma once

#include "../Phsyics/Body.h"

class Ship
{
public:
    Ship();
	Ship(const float& x, const float& y, const int& radius, const float& mass);
	~Ship();

    void Update(float dt);
    void Render();

private:
	Body* body;

    float throttle = 0.0f;
    float throttleMax = 1.0f;
    float throttleRate = 0.5f;
    float thrust = 1.0f;
    Vec2 thrustVector = { 0.0f, -1.0f };

    float turnSpeed = 1.0f;

    float fuel = 1.0f;
    float fuelMaxBurnRate = 0.01f;
};