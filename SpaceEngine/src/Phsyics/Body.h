#pragma once

#include "Shape.h"
#include "Vec2.h"

struct Body
{
    bool resolvePentration = true;

    Shape* shape = nullptr;

    float mass;
    float invMass;
    float I;
    float invI;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float rotation;
    float angularVelocity;
    float angularAcceleration;

    float restitution;
    
    Vec2 sumForces;
    float sumTorque;

    Body(const Shape& shape, float x, float y, float mass);
    Body(const Body& other);
    ~Body();

    bool IsStatic() const;

    void Update(float dt);
    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    void ClearForces();
    void ClearTorque();

    void AddForce(const Vec2& force);
    void AddTorque(const float& torque);

    void ApplyImpulse(const Vec2& j);
};