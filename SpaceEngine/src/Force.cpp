#include "Force.h"

Vec2 Force::GenerateDragForce(const Body& particle, float k)
{
    Vec2 dragForce = { 0.0f, 0.0f };

    if (particle.velocity.MagnitudeSquared() > 0)
    {
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;

        float dragMagnitutde = k * particle.velocity.MagnitudeSquared();

        dragForce = dragDirection * dragMagnitutde;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body& particle, float k)
{
    Vec2 frictionForce = Vec2(0, 0);

    Vec2 frictionDirection = particle.velocity.UnitVector() * -1.0;

    float frictionMagnitude = k;

    frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance)
{
    Vec2 d = (b.position - a.position);

    float distanceSquared = d.MagnitudeSquared();
    if (distanceSquared > maxDistance) distanceSquared = maxDistance;
    else if (distanceSquared < minDistance) distanceSquared = minDistance;

    Vec2 attractionDirection = d.UnitVector();
    float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

    Vec2 attractionForce = attractionDirection * attractionMagnitude;

    return attractionForce;
}
