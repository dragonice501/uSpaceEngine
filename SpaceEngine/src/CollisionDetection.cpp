#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle)
        return IsCollidingCircleCircle(a, b, contact);

    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact)
{
    CircleShape* aCircleShape = static_cast<CircleShape*>(a->shape);
    CircleShape* bCircleShape = static_cast<CircleShape*>(b->shape);

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    if (ab.MagnitudeSquared() > radiusSum * radiusSum) return false;

    contact.a = a;
    contact.b = b;

    contact.normal = ab;
    contact.normal.Normalize();
   
    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;

    contact.depth = (contact.start - contact.end).Magnitude();

    return true;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
    return false;
}

bool CollisionDetection::IsCollidingPolygonCircle(Body* a, Body* b, Contact& contact)
{
    return false;
}
