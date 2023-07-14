#include "Ship.h"
#include "../Graphics/Graphics.h"
#include "../Phsyics/Force.h"
#include "../Phsyics/PhysicsConstants.h"

Ship::Ship()
{
}

Ship::Ship(const float& x, const float& y, const int& radius, const float& mass)
{
	body = new Body(CircleShape(radius), x, y, mass);
}

Ship::~Ship()
{
	delete body;
}

void Ship::Update(float dt, const Satellite* satellite)
{
    if (throttleForwardPressed)
    {
        throttle += throttleRate * dt;

        if (throttle > throttleMax) throttle = throttleMax;
    }
    else if (throttleBackPressed)
    {
        throttle -= throttleRate * dt;

        if (throttle < 0.0f) throttle = 0.0f;
    }

    if (turnRightPressed)
    {
        thrustVector = thrustVector.Rotate(M_PI * turnSpeed * dt);
    }
    if (turnLeftPressed)
    {
        thrustVector = thrustVector.Rotate(-M_PI * turnSpeed * dt);
    }

    Vec2 attraction = Force::GenerateGravitationalForce(*body, *satellite->GetBody(), G, 0, 100);
    body->AddForce(attraction);

    if (throttle != 0.0f && fuel != 0.0f)
    {
        body->AddForce(thrustVector * thrust * throttle);
        fuel -= fuelMaxBurnRate * throttle * dt;
        if (fuel < 0.0f) fuel = 0.0f;
    }

    body->Update(dt);
}

void Ship::Render()
{
}
