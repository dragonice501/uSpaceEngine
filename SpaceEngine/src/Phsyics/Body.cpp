#include "Body.h"
#include "../Graphics//GraphicsConstants.h"

Body::Body(const Shape& shape, float x, float y, float mass)
{
    this->shape = shape.Clone();

    this->mass = mass;
    if (mass != 0.0)
        this->invMass = 1.0f / mass;
    else
        this->invMass = 0.0f;

    this->I = shape.GetMomentOfInertia() * mass;
    if (this->I != 0.0)
        this->invI = 1.0 / this->I;
    else
        this->invI = 0.0f;

    this->position = Vec2(x, y);

    this->velocity = Vec2(0.0f, 0.0f);
    this->acceleration = Vec2(0.0f, 0.0f);

    this->rotation = 0.0f;
    this->angularVelocity = 0.0f;
    this->angularAcceleration = 0.0f;

    this->restitution = 1.0f;

    this->sumForces = Vec2(sumForces);
    this->sumTorque = 0.0f;
}

Body::Body(const Body& other)
{
    this->shape = other.shape->Clone();

    this->mass = other.mass;
    this->invMass = other.mass;
    this->I = other.I;
    this->invI = other.invI;

    this->position = other.position;
    this->velocity = other.velocity;
    this->acceleration = other.acceleration;

    this->rotation = other.rotation;
    this->angularAcceleration = other.angularAcceleration;
    this->angularVelocity = other.angularVelocity;
}

Body::~Body()
{
    delete shape;
}

bool Body::IsStatic() const
{
    const float epsilon = 0.005f;
    return fabs(invMass - 0.0f) < epsilon;
}

void Body::Update(float dt)
{
    IntegrateLinear(dt);
    IntegrateAngular(dt);

    if (shape->GetType() == POLYGON || shape->GetType() == BOX)
    {
        PolygonShape* polyShape = static_cast<PolygonShape*>(shape);
        polyShape->UpdateVertices(rotation, position);
    }
}

void Body::IntegrateLinear(float dt)
{
    if (IsStatic()) return;

    acceleration = sumForces * invMass;

    velocity += acceleration * dt;
    position += velocity * dt;

    ClearForces();
}

void Body::IntegrateAngular(float dt)
{
    if (IsStatic()) return;

    angularAcceleration = sumTorque * invI;

    angularVelocity += angularAcceleration * dt;
    rotation += angularVelocity * dt;

    ClearTorque();
}

void Body::ClearForces()
{
    sumForces = Vec2(0.0f, 0.0f);
}

void Body::ClearTorque()
{
    sumTorque = 0.0f;
}

void Body::AddForce(const Vec2& force)
{
    sumForces += force;
}

void Body::AddTorque(const float& torque)
{
    sumTorque += torque;
}

void Body::ApplyImpulse(const Vec2& j)
{
    if (IsStatic()) return;

    velocity *= 0.9f;
    velocity += j * invMass;
}
