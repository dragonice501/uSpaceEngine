#pragma once

#include "../Phsyics/Body.h"
#include "Satellite.h"

class Ship
{
public:
    Ship();
	Ship(const float& x, const float& y, const int& radius, const float& mass);
	~Ship();

    inline Body* GetBody() { return body; }

    inline const float& GetThrottle() const { return throttle; }
    inline const Vec2& GetThrustVector() const { return thrustVector; }
    inline const float& GetFuel() const { return fuel; }

    inline void SetThrottle(const float& newThrottle) { throttle = newThrottle; }
    inline void SetThrustVector(const Vec2& vector) { thrustVector = vector; }
    inline void SetFuel(const float& newFuel) { fuel = newFuel; }

    inline void ThrottleForwardPressed(const bool& pressed) { throttleForwardPressed = pressed; }
    inline void ThrottleBackPressed(const bool& pressed) { throttleBackPressed = pressed; }
    inline void TurnRightPressed(const bool& pressed) { turnRightPressed = pressed; }
    inline void TurnLeftPressed(const bool& pressed) { turnLeftPressed = pressed; }

    void Update(const float& dt, const Satellite* satellite);
    void Render(const bool& showVectors, const bool& showTrajectory, const Satellite* satellite);

private:
	Body* body;

    bool throttleForwardPressed = false;
    bool throttleBackPressed = false;
    bool turnRightPressed = false;
    bool turnLeftPressed = false;

    float throttle = 0.0f;
    float throttleMax = 1.0f;
    float throttleRate = 0.5f;
    float thrust = 1.0f;
    Vec2 thrustVector = { 0.0f, -1.0f };

    float turnSpeed = 0.5f;

    float fuel = 1.0f;
    float fuelMaxBurnRate = 0.01f;
};