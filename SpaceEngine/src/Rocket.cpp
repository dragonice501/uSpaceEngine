#include "Rocket.h"

void Rocket::Update(const float dt, const CelestialBody& centralBody)
{
    Vec3Double distance = (centralBody.position - position);

    alttitude = distance.Magnitude() - centralBody.radius - radius;

    Vec3Double attractionDirection = distance.Normal();

    double attractionMagnitude = (G * (centralBody.mass + mass)) / distance.MagnitudeSquared();
    gravity = attractionMagnitude;

    forcesVector += attractionDirection * attractionMagnitude;

    velocity += forcesVector * dt;
    position += velocity * dt;

    forcesVector = Vec3Double();
}
