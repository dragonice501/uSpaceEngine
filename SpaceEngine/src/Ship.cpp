#include "Ship.h"
#include "Graphics.h"
#include "Force.h"
#include "PhysicsConstants.h"
#include "Contact.h"
#include "CollisionDetection.h"

Ship::Ship()
{
}

Ship::Ship(const float& x, const float& y, const int& radius, const float& mass, Satellite* influencingSatellite)
{
	body = new Body(CircleShape(radius), x, y, mass);
    this->influencingSatellite = influencingSatellite;
}

Ship::~Ship()
{
	delete body;
}

void Ship::Update(const float& dt)
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

    if (influencingSatellite)
    {
        Vec2 attraction = Force::GenerateGravitationalForce(*body, *influencingSatellite->GetBody(), G, 0, influencingSatellite->GetSOI());
        body->AddForce(attraction);
    }

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
    CircleShape* circle = static_cast<CircleShape*>(body->shape);
    if (showVectors && body->shape->GetType() == CIRCLE)
    {
        Vec2 velocityVector = body->velocity;
        velocityVector.Normalize();

        Graphics::DrawLine(
            body->position.x,
            body->position.y,
            body->position.x + velocityVector.x * circle->radius * 3.0f,
            body->position.y + velocityVector.y * circle->radius * 3.0f,
            0xFFFFFF00, false);

        Graphics::DrawLine(
            body->position.x,
            body->position.y,
            body->position.x + GetThrustVector().x * circle->radius * 3.0f,
            body->position.y + GetThrustVector().y * circle->radius * 3.0f,
            0xFFFF0000, false);
    }

    if (influencingSatellite)
    {
        CircleShape* circle = static_cast<CircleShape*>(body->shape);
        Vec2 vector = influencingSatellite->GetBody()->position - body->position;
        vector.Normalize();

        /*Graphics::DrawLine(
            body->position.x,
            body->position.y,
            body->position.x + vector.x * circle->radius * 3.0f,
            body->position.y + vector.y * circle->radius * 3.0f,
            influencingSatellite->GetColor(), false);*/

        if (showTrajectory)
        {
            Body dummyBody(*body);
            Body dummyMoon(*influencingSatellite->GetBody());

            Vec2 attraction;
            Vec2 acceleration;

            Vec2 lastPosition;

            Contact contact;

            for (int i = 0; i < 500; i++)
            {
                lastPosition = dummyBody.position;

                attraction = Force::GenerateGravitationalForce(dummyBody, dummyMoon, G, 0, influencingSatellite->GetSOI());

                dummyBody.acceleration = attraction * dummyBody.invMass;
                dummyBody.velocity += dummyBody.acceleration * 0.5f;
                dummyBody.position += dummyBody.velocity * 0.5f;

                dummyMoon.position += dummyMoon.velocity;

                if (CollisionDetection::IsColliding(&dummyBody, &dummyMoon, contact)) break;

                Graphics::DrawPixel(dummyBody.position.x + Graphics::screenOffset.x, dummyBody.position.y + Graphics::screenOffset.y, 0xFFAAAAFF);
            }
        }
    }

    if (body->shape->GetType() == CIRCLE)
    {
        Graphics::DrawFillCircle(body->position.x, body->position.y, circle->radius, 0xFFFFAA00);
    }
}

void Ship::CheckCollision()
{
    if (influencingSatellite)
    {
        Contact contact;
        if (CollisionDetection::IsCollidingCircleCircle(body, influencingSatellite->GetBody(), contact))
        {
            contact.ResolveCollision();
        }
    }
}
